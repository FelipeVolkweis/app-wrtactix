#include "utils/testsuite/testsuite.hh"
#include "algorithm/goap/goap.hh"
#include "algorithm/goap/behavior.hh"
#include "algorithm/goap/goal.hh"

#include <iostream>

// Mock Behaviors for testing
class MockBehavior : public Behavior {
public:
    MockBehavior(QString name, float cost, 
                 Dictionary<bool> preconditions, 
                 Dictionary<bool> effects)
        : cost_(cost), 
          preconditions_(preconditions), 
          effects_(effects), Behavior(name) {}

    bool isValid() const override { return true; }
    float cost() const override { return cost_; }
    Dictionary<bool> preconditions() const override { return preconditions_; }
    Dictionary<bool> effects() const override { return effects_; }
    Status execute() const override { return Status::SUCCESS; }

private:
    float cost_;
    Dictionary<bool> preconditions_;
    Dictionary<bool> effects_;
};

// Mock Goals for testing
class MockGoal : public Goal {
public:
    MockGoal(int priority, Dictionary<bool> desiredState)
        : priority_(priority), desiredState_(desiredState), Goal("Mock") {}

    bool isValid() const override { return true; }
    int priority() const override { return priority_; }
    Dictionary<bool> desiredWorldState() const override { return desiredState_; }

private:
    int priority_;
    Dictionary<bool> desiredState_;
};

class TestGOAP : public TestSuite {
    Q_OBJECT
public:
    using TestSuite::TestSuite;

private slots:
    void initTestCase();
    void cleanupTestCase();
    
    void testEmptyGoal();
    void testDirectSolution();
    void testMultiStepSolution();
    void testNoSolution();
    void testCheapestPlanSelection();
    void testInvalidBehaviorsAreSkipped();
    
private:
    QVector<Behavior*> createTestBehaviors();
    Dictionary<bool> createTestBlackboard();
};

void TestGOAP::initTestCase() {
    // Initialization if needed
}

void TestGOAP::cleanupTestCase() {
    // Cleanup if needed
}

QVector<Behavior*> TestGOAP::createTestBehaviors() {
    // Let's model a simple "get food" scenario
    
    // Behavior 1: Get raw food (requires nothing, gives raw food)
    Dictionary<bool> noPreconditions;
    Dictionary<bool> rawFoodEffect;
    rawFoodEffect["has_raw_food"] = true;
    auto* getRawFood = new MockBehavior("GetRawFood", 1.0f, noPreconditions, rawFoodEffect);
    
    // Behavior 2: Cook food (requires raw food, gives cooked food)
    Dictionary<bool> needsRawFood;
    needsRawFood["has_raw_food"] = true;
    Dictionary<bool> cookedFoodEffect;
    cookedFoodEffect["has_cooked_food"] = true;
    auto* cookFood = new MockBehavior("CookFood", 2.0f, needsRawFood, cookedFoodEffect);
    
    // Behavior 3: Eat food (requires cooked food, gives not hungry)
    Dictionary<bool> needsCookedFood;
    needsCookedFood["has_cooked_food"] = true;
    Dictionary<bool> notHungryEffect;
    notHungryEffect["is_hungry"] = false;
    auto* eatFood = new MockBehavior("EatFood", 1.5f, needsCookedFood, notHungryEffect);
    
    // Behavior 4: Buy food (requires money, gives raw food)
    Dictionary<bool> needsMoney;
    needsMoney["has_money"] = true;
    auto* buyFood = new MockBehavior("BuyFood", 3.0f, needsMoney, rawFoodEffect);
    
    // Behavior 5: Work (requires nothing, gives money)
    Dictionary<bool> moneyEffect;
    moneyEffect["has_money"] = true;
    auto* work = new MockBehavior("Work", 5.0f, noPreconditions, moneyEffect);
    
    return {getRawFood, cookFood, eatFood, buyFood, work};
}

Dictionary<bool> TestGOAP::createTestBlackboard() {
    Dictionary<bool> blackboard;
    blackboard["is_hungry"] = true;
    return blackboard;
}

void TestGOAP::testEmptyGoal() {
    QVector<Behavior*> behaviors = createTestBehaviors();
    GOAP planner(behaviors);
    Dictionary<bool> blackboard = createTestBlackboard();
    
    // Empty goal
    Dictionary<bool> emptyDesiredState;
    MockGoal emptyGoal(1, emptyDesiredState);
    
    Plan plan = planner.getPlan(emptyGoal, blackboard);
    QVERIFY(plan.behaviors.isEmpty());
    QCOMPARE(plan.cost, INFINITY);
    
    qDeleteAll(behaviors);
}

void TestGOAP::testDirectSolution() {
    QVector<Behavior*> behaviors = createTestBehaviors();
    GOAP planner(behaviors);
    Dictionary<bool> blackboard = createTestBlackboard();
    
    // Goal that can be satisfied by a single behavior
    Dictionary<bool> wantRawFood;
    wantRawFood["has_raw_food"] = true;
    MockGoal rawFoodGoal(1, wantRawFood);
    
    Plan plan = planner.getPlan(rawFoodGoal, blackboard);
    QCOMPARE(plan.behaviors.size(), 1);
    QCOMPARE(static_cast<MockBehavior*>(plan.behaviors[0])->name(), QString("GetRawFood"));
    QCOMPARE(plan.cost, 1.0f);
    
    qDeleteAll(behaviors);
}

void TestGOAP::testMultiStepSolution() {
    QVector<Behavior*> behaviors = createTestBehaviors();
    GOAP planner(behaviors);
    Dictionary<bool> blackboard = createTestBlackboard();
    
    // Goal that requires multiple steps
    Dictionary<bool> notHungryGoalState;
    notHungryGoalState["is_hungry"] = false;
    MockGoal notHungryGoal(1, notHungryGoalState);
    
    Plan plan = planner.getPlan(notHungryGoal, blackboard);
    QVERIFY(plan.behaviors.size() >= 2); // At least GetRawFood + CookFood + EatFood
    
    // Check the plan makes sense
    bool hasGetRawFood = false;
    bool hasCookFood = false;
    bool hasEatFood = false;
    
    for (Behavior* b : plan.behaviors) {
        QString name = static_cast<MockBehavior*>(b)->name();
        if (name == "GetRawFood") hasGetRawFood = true;
        if (name == "CookFood") hasCookFood = true;
        if (name == "EatFood") hasEatFood = true;
    }
    
    QVERIFY(hasGetRawFood);
    QVERIFY(hasCookFood);
    QVERIFY(hasEatFood);
    
    qDeleteAll(behaviors);
}

void TestGOAP::testNoSolution() {
    QVector<Behavior*> behaviors = createTestBehaviors();
    GOAP planner(behaviors);
    Dictionary<bool> blackboard = createTestBlackboard();
    
    // Impossible goal
    Dictionary<bool> impossibleGoalState;
    impossibleGoalState["has_spaceship"] = true;
    MockGoal impossibleGoal(1, impossibleGoalState);
    
    Plan plan = planner.getPlan(impossibleGoal, blackboard);
    QVERIFY(plan.behaviors.isEmpty());
    QCOMPARE(plan.cost, INFINITY);
    
    qDeleteAll(behaviors);
}

void TestGOAP::testCheapestPlanSelection() {
    QVector<Behavior*> behaviors = createTestBehaviors();
    GOAP planner(behaviors);
    Dictionary<bool> blackboard = createTestBlackboard();
    
    // Goal that can be achieved in two ways
    Dictionary<bool> wantRawFood;
    wantRawFood["has_raw_food"] = true;
    MockGoal rawFoodGoal(1, wantRawFood);
    
    // There are two ways to get raw food:
    // 1. GetRawFood directly (cost 1.0)
    // 2. Work -> BuyFood (cost 5.0 + 3.0 = 8.0)
    // The planner should choose the cheaper option
    
    Plan plan = planner.getPlan(rawFoodGoal, blackboard);
    QCOMPARE(plan.behaviors.size(), 1);
    QCOMPARE(static_cast<MockBehavior*>(plan.behaviors[0])->name(), QString("GetRawFood"));
    QCOMPARE(plan.cost, 1.0f);
    
    qDeleteAll(behaviors);
}

void TestGOAP::testInvalidBehaviorsAreSkipped() {
    // Create behaviors where one is invalid
    Dictionary<bool> noPreconditions;
    Dictionary<bool> effect;
    effect["has_item"] = true;
    
    auto* validBehavior = new MockBehavior("Valid", 1.0f, noPreconditions, effect);
    auto* invalidBehavior = new MockBehavior("Invalid", 1.0f, noPreconditions, effect);
    
    // Make the invalid behavior return false for isValid()
    class InvalidMockBehavior : public MockBehavior {
    public:
        using MockBehavior::MockBehavior;
        bool isValid() const override { return false; }
    };
    invalidBehavior = new InvalidMockBehavior("Invalid", 1.0f, noPreconditions, effect);
    
    QVector<Behavior*> behaviors = {validBehavior, invalidBehavior};
    GOAP planner(behaviors);
    
    Dictionary<bool> blackboard;
    Dictionary<bool> goalState;
    goalState["has_item"] = true;
    MockGoal testGoal(1, goalState);
    
    Plan plan = planner.getPlan(testGoal, blackboard);
    QCOMPARE(plan.behaviors.size(), 1);
    QCOMPARE(static_cast<MockBehavior*>(plan.behaviors[0])->name(), QString("Valid"));
    
    qDeleteAll(behaviors);
}

static TestGOAP TEST_GOAP;
#include "test_goap.moc"