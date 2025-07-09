#include "attackersupport.hh"
#include "agent/sslagent/behaviors/shoottogoal.hh"
#include "agent/sslagent/behaviors/pass.hh"
#include "agent/sslagent/behaviors/receivepass.hh"

AttackerSupport::AttackerSupport(const PlayerID &playerId, SSLController &controller, const World &worldRef) 
    : SSLBehavior(playerId, controller, worldRef, "SupportAttacker") {
    auto root = BehaviorTree::Fallback("AttackerSupportFallback", {
        BehaviorTree::Sequence("PositionForPass", {
            BehaviorTree::Condition("ShouldReceivePass", 
                [this]() {
                    return !ballInteraction().isOurClosestToBallByMargin(player(), 0.2f);
                }),
            BehaviorTree::ForceSuccess((new ReceivePass(playerId, controller, worldRef))->root())
        }),
        BehaviorTree::Sequence("PassIfNeeded", {
            BehaviorTree::Condition("CannotKickToGoal", 
                [this]() {
                    bool flag = false;
                    aiming().getEnemyGoalKickPosition(player(), flag);
                    return !flag;
                }),
            BehaviorTree::ForceSuccess((new Pass(playerId, controller, worldRef))->root())
        }),
        (new ShootToGoal(playerId, controller, worldRef))->root()
    });

    setRoot(root);
}