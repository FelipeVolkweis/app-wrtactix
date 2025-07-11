#include "supportattacker.hh"
#include "agent/sslagent/behaviors/shoottogoal.hh"
#include "agent/sslagent/behaviors/pass.hh"
#include "agent/sslagent/behaviors/receivepass.hh"

SupportAttacker::SupportAttacker(const PlayerID &playerId, SSLController &controller, const World &worldRef) 
    : SSLBehavior(playerId, controller, worldRef, "SupportAttacker") {
    auto root = BehaviorTree::Fallback("SupportAttackerFallback", {
        BehaviorTree::Sequence("ShootingSequence", {
            BehaviorTree::Condition("HasBallPossession", 
                [this]() {
                    return ballInteraction().isOurClosestToBallByMargin(player(), 0.6f);
                }),
            BehaviorTree::Condition("CanShootToGoal", 
                [this] () {
                    bool flag = false;
                    aiming().getEnemyGoalKickPosition(player(), flag);
                    return flag;
                }),
            BehaviorTree::Condition(
                "IsTheBallComingAtMe",
                [this]() {
                    return !ballInteraction().isTheBallInCollisionRoute(world().playerPositionVec2(player()));
                }),
            BehaviorTree::ForceSuccess((new ShootToGoal(playerId, controller, worldRef))->root())
        }),
        BehaviorTree::Sequence("PassingSequence", {
            BehaviorTree::Condition("HasBallPossession", 
                [this]() {
                    return ballInteraction().isOurClosestToBallByMargin(player(), 0.6f);
                }),
            BehaviorTree::Condition("CanTheReceiverTrapTheBall",
                [this]() {
                    auto receiver = aiming().getBestReceiver();
                    return aiming().canTheReceiverTrapTheBall(player(), receiver);
                }),
            BehaviorTree::ForceSuccess((new Pass(playerId, controller, worldRef))->root())
        }),
        (new ReceivePass(playerId, controller, worldRef))->root()
    });

    setRoot(root);
}