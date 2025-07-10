#include "receivepass.hh"

#include "algorithm/pathplanner/pflorinho/pflorinho.hh"

Q_LOGGING_CATEGORY(RECEIVEPASS, "ReceivePass")

ReceivePass::ReceivePass(const PlayerID &playerId, SSLController &controller, const World &worldRef) : 
    SSLBehavior(playerId, controller, worldRef, "ReceivePass") {
    auto root = BehaviorTree::Fallback("ReceivePassFallback", {
            BehaviorTree::Sequence("PositionToReceiveSequence", {
                BehaviorTree::Condition("IsTheBallNotComingAtMe",
                    [this]() {
                        return !ballInteraction().isTheBallInCollisionRoute(world().playerPositionVec2(player()));
                    }),
                BehaviorTree::ForceSuccess(
                    action<GoToLookAt>()
                        ->setPathPlanner(new PFLorinho())
                        ->setGoal([this]() {
                            if (player() == PlayerID(world().ourColor(), 4))
                                return Vec2(-0.5, 0);
                            return Vec2(-1.5, -1);
                        })
                        ->setLookAt([this]() {
                            bool hasValidOpening = false;
                            auto posToDeflectTo = aiming().getEnemyGoalDeflectPosition(player(), hasValidOpening);
                            
                            if (hasValidOpening) {
                                return posToDeflectTo;
                            }
                            return world().ballPositionVec2();
                        })
                        ->avoidTeammates()
                        ->avoidOpponents()
                        ->avoidTheirGoal()
                        ->avoidBall()
                ),
            }),
            BehaviorTree::Sequence("PositionToDeflectSequence", {
                BehaviorTree::Condition("CanTheBallBeDeflected",
                    [this]() {
                        bool hasValidOpening = false;
                        aiming().getEnemyGoalDeflectPosition(player(), hasValidOpening);
                        bool isComingAtMe = ballInteraction().isTheBallInCollisionRoute(world().playerPositionVec2(player()));
                        return hasValidOpening && isComingAtMe;
                    }),
                action<Kick>()
                    ->setPower(10),
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