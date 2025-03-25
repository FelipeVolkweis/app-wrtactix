#include "goap.hh"
#include <iostream>

GOAP::GOAP(QVector<Behavior *> availableBehaviors) : availableBehaviors_(availableBehaviors) {}

Plan GOAP::getPlan(const Goal &goal, const Dictionary<bool> &blackboard) {
    if (!goal.isValid() || goal.desiredWorldState().isEmpty()) {
        return Plan();
    }

    return getBestPlan(goal, blackboard);
}

bool GOAP::buildPlan(QSharedPointer<GoapNode> step, const Dictionary<bool> &blackboard) {
    bool hasFollowUp = false;
    auto state = step->desiredState();

    for (const auto &s : state.asKeyValueRange()) {
        if (blackboard.contains(s.first) && state[s.first] == blackboard[s.first]) {
            state.remove(s.first);
        }
    }

    if (state.isEmpty()) {
        return true;
    }

    for (auto &b : availableBehaviors_) {
        if (!b->isValid()) {
            continue;
        }

        bool shouldAddToPlan = false;
        auto effects = b->effects();
        auto desiredState = state;

        for (const auto &s : desiredState.asKeyValueRange()) {
            if (effects.contains(s.first) && effects[s.first] == desiredState[s.first]) {
                desiredState.remove(s.first);
                shouldAddToPlan = true;
            }
        }

        if (shouldAddToPlan) {
            for (const auto &precon : b->preconditions().asKeyValueRange()) {
                const auto &s = precon.first;
                desiredState[s] = b->preconditions()[s];
            }

            QSharedPointer<GoapNode> node(new GoapNode(b, desiredState));

            if (desiredState.isEmpty() || buildPlan(node, blackboard)) {
                step->addChild(node);
                hasFollowUp = true;
            }
        }
    }

    return hasFollowUp;
}

QVector<Plan> GOAP::getAllPlansFromTree(GoapNode *root, const Dictionary<bool> &blackboard) {
    QVector<Plan> plans;

    if (root == nullptr) {
        return QVector<Plan>();
    }

    if (root->isLeaf()) {
        plans.push_back({QVector<Behavior *>{root->behavior()}, root->behavior()->cost()});
        return plans;
    }

    for (auto &c : root->getChildren()) {
        for (auto &newPlan : getAllPlansFromTree(c.data(), blackboard)) {
            if (root->hasBehavior()) {
                newPlan.behaviors.append(root->behavior());
                newPlan.cost += root->behavior()->cost();
            }
            plans.append(newPlan);
        }
    }

    return plans;
}

Plan GOAP::getBestPlan(const Goal &goal, const Dictionary<bool> &blackboard) {
    QSharedPointer<GoapNode> node(new GoapNode(goal.desiredWorldState()));

    if (buildPlan(node, blackboard)) {
        auto plans = getAllPlansFromTree(node.data(), blackboard);
        return getCheapestPlan(plans);
    }

    return Plan();
}

Plan GOAP::getCheapestPlan(const QVector<Plan> &plans) {
    Plan best;

    for (const auto &p : plans) {
        if (p.cost < best.cost) {
            best = p;
        }
    }

    return best;
}
