#include "agent/sslagent/actions/actions.hh"
#include "algorithm/behaviortree/behaviortree.hh"
#include "algorithm/pathplanner/pointtopoint/pointtopoint.hh"

#include "calibratelinear.hh"

CalibrateLinear::CalibrateLinear(const PlayerID &playerId, SSLController &controller, const World &worldRef)
    : SSLBehavior(playerId, controller, worldRef, "CalibrateLinear") {
    Vec2 A = {-0.5f, -2.3f};
    Vec2 B = {-2.7f, -2.3f};
    Vec2 C = {-2.7f, 2.3f};
    Vec2 D = {-0.5f, 2.3f};

    auto root = BehaviorTree::Sequence(
        "Calibrate Linear Sequence",
        {
            BehaviorTree::Fallback("FallbA", {
                BehaviorTree::Condition("Check A", [this]() { return beenToA_; }),
                action<GoTo>()
                    ->setGoal([A]() {
                        return A;
                    })
                    ->setPathPlanner(new PointToPoint())
            }),
            BehaviorTree::Fallback("FallbB", {
                BehaviorTree::ForceFailure(
                    BehaviorTree::RunLambda("Set BeenToA", [this]() {
                        beenToA_ = true;
                    })
                ),
                BehaviorTree::Condition("Check B", [this]() { return beenToB_; }),
                action<GoTo>()
                    ->setGoal([B]() {
                        return B;
                    })
                    ->setPathPlanner(new PointToPoint())
            }),
            BehaviorTree::Fallback("FallbC", {
                BehaviorTree::ForceFailure(
                    BehaviorTree::RunLambda("Set BeenToB", [this]() {
                        beenToB_ = true;
                    })
                ),
                BehaviorTree::Condition("Check C", [this]() { return beenToC_; }),
                action<GoTo>()
                    ->setGoal([C]() {
                        return C;
                    })
                    ->setPathPlanner(new PointToPoint())
            }),
            BehaviorTree::Fallback("FallbD", {
                BehaviorTree::ForceFailure(
                    BehaviorTree::RunLambda("Set BeenToC", [this]() {
                        beenToC_ = true;
                    })
                ),
                BehaviorTree::Condition("Check D", [this]() { return beenToD_; }),
                action<GoTo>()
                    ->setGoal([D]() {
                        return D;
                    })
                    ->setPathPlanner(new PointToPoint())
            }),
            BehaviorTree::RunLambda("Set BeenToD", [this]() {
                beenToD_ = true;
            }),
            BehaviorTree::Condition("Check Done", [this]() {
                return beenToA_ && beenToB_ && beenToC_ && beenToD_;
            }),
            BehaviorTree::RunLambda("Reset", [this]() {
                beenToA_ = false;
                beenToB_ = false;
                beenToC_ = false;
                beenToD_ = false;
            })
        });

    setRoot(root);
}