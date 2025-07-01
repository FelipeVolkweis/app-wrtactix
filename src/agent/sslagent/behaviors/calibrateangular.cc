#include "agent/sslagent/actions/actions.hh"
#include "algorithm/behaviortree/behaviortree.hh"

#include "calibrateangular.hh"

CalibrateAngular::CalibrateAngular(const PlayerID &playerId, SSLController &controller, const World &worldRef)
    : SSLBehavior(playerId, controller, worldRef, "CalibrateAngular") {
    auto root = BehaviorTree::Sequence(("AngularCalibration"),
    {
        BehaviorTree::Fallback("FallbA", {
            BehaviorTree::Condition("Check A", [this]() { return beenToA_; }),
            action<LookAt>()
                ->setLookAt([this]() {
                    auto pos = world().playerPositionVec2(player());
                    pos.x() += 1.0f; 
                    return pos;
                })
        }),
        BehaviorTree::Fallback("FallbB", {
            BehaviorTree::ForceFailure(
                BehaviorTree::RunLambda("Set BeenToA", [this]() {
                    beenToA_ = true;
                })
            ),
            BehaviorTree::Condition("Check B", [this]() { return beenToB_; }),
            action<LookAt>()
                ->setLookAt([this]() {
                    auto pos = world().playerPositionVec2(player());
                    pos.y() += 1.0f;
                    return pos;
                })
        }),
        BehaviorTree::Fallback("FallbC", {
            BehaviorTree::ForceFailure(
                BehaviorTree::RunLambda("Set BeenToB", [this]() {
                    beenToB_ = true;
                })
            ),
            BehaviorTree::Condition("Check C", [this]() { return beenToC_; }),
            action<LookAt>()
                ->setLookAt([this]() {
                    auto pos = world().playerPositionVec2(player());
                    pos.x() -= 1.0f;
                    return pos;
                })
        }),
        BehaviorTree::Fallback("FallbD", {
            BehaviorTree::ForceFailure(
                BehaviorTree::RunLambda("Set BeenToC", [this]() {
                    beenToC_ = true;
                })
            ),
            BehaviorTree::Condition("Check D", [this]() { return beenToD_; }),
            action<LookAt>()
                ->setLookAt([this]() {
                    auto pos = world().playerPositionVec2(player());
                    pos.y() -= 1.0f;
                    return pos;
                })
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