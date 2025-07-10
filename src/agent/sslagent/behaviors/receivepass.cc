#include "receivepass.hh"

#include "algorithm/pathplanner/pflorinho/pflorinho.hh"

Q_LOGGING_CATEGORY(RECEIVEPASS, "ReceivePass")

ReceivePass::ReceivePass(const PlayerID &playerId, SSLController &controller, const World &worldRef) : 
    SSLBehavior(playerId, controller, worldRef, "ReceivePass") {
    auto root = BehaviorTree::Sequence("ReceivePassSequence", {
            BehaviorTree::Fallback("PositionToReceiveFallback", {
                BehaviorTree::Condition("IsTheBallComingAtMe",
                    [this]() {
                        return ballInteraction().isTheBallInCollisionRoute(world().playerPositionVec2(player()));
                    }),
                action<GoToLookAt>()
                    ->setPathPlanner(new PFLorinho())
                    ->setGoal([this]() {
                        if (player() == PlayerID(0, 1))
                            return Vec2(-0.5, 0);
                        return Vec2(-1.5, -1);
                    })
                    ->setLookAt([this]() {
                        return world().ballPositionVec2();
                    })
                    ->avoidTeammates()
                    ->avoidOpponents()
                    ->avoidTheirGoal()
                    ->avoidBall()
            }),
            BehaviorTree::Fallback("PositionToDeflectFallback", {
                BehaviorTree::Condition("CanTheBallBeDeflected",
                    [this]() {
                        bool hasValidOpening = false;
                        aiming().getEnemyGoalDeflectPosition(player(), hasValidOpening);
                        qCInfo(RECEIVEPASS) << hasValidOpening;
                        return !hasValidOpening;
                    }),
                BehaviorTree::ForceFailure(
                    action<Kick>()
                        ->setPower(10)
                ),
                BehaviorTree::ForceSuccess(
                    action<GoToLookAt>()
                        ->setPathPlanner(new PFLorinho())
                        ->setGoal([this]() {
                            return ballInteraction().getPositionToInterceptMovingBall(player());
                        })
                        ->setLookAt([this]() {
                            return  aiming().getEnemyGoalDeflectPosition(player());
                        })
                        ->avoidTeammates()
                        ->avoidOpponents()
                        ->avoidTheirGoal()
                        ->avoidBall()
                )
            }),
            action<GoToLookAt>()
                ->setPathPlanner(new PFLorinho())
                ->setGoal([this]() {
                    return ballInteraction().getPositionToInterceptMovingBall(player());
                })
                ->setLookAt([this]() {
                    auto pos = world().playerPositionVec2(player());
                    pos -= world().ballVelocityVec2().normalized();
                    return pos;
                })
                ->avoidTeammates()
                ->avoidOpponents()
                ->avoidTheirGoal()
                ->avoidBall(),
        }
    );
     
    setRoot(root);
}