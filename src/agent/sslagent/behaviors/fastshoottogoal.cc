#include "algorithm/pathplanner/pflorinho/pflorinho.hh"
#include "algorithm/pathplanner/pointtopoint/pointtopoint.hh"
#include "constants/constants.hh"

#include "fastshoottogoal.hh"

FastShootToGoal::FastShootToGoal(const PlayerID &playerId, SSLController &controller, const World &worldRef)
    : SSLBehavior(playerId, controller, worldRef, "FastShootToGoal") {
    auto root = BehaviorTree::Sequence("Shooting Sequence", {
        BehaviorTree::Fallback("PositionToShootFallback", {
            BehaviorTree::Condition("IsBehindBall",
                                    [this]() {
                                        return ballInteraction().isBehindBall(
                                            world().playerPositionVec2(player()),
                                            aiming().getEnemyGoalKickPosition(player()), Const::Skills::Dribbling::behind_ball_distance);
                                    }),
            action<GoToLookAt>()
                ->setPathPlanner(new PFLorinho())
                ->setGoal([this]() {
                    return ballInteraction().behindBall(aiming().getEnemyGoalKickPosition(player()), Const::Skills::Kicking::optimal_distance);
                })
                ->setLookAt([this]() { return aiming().getEnemyGoalKickPosition(player()); })
                ->avoidTeammates()
                ->avoidOpponents()
                ->avoidTheirGoal()
                ->avoidBall([this]() {
                    return !ballInteraction().isBehindBall(world().playerPositionVec2(player()),
                                                        aiming().getEnemyGoalKickPosition(player()), 0.4f,
                                                        0.9f);
                })
                ->setSpeedBoost([this]() {
                    return Const::Skills::Dribbling::max_push_distance;
                }),
        }),
        action<Kick>()->setPower(10.f),
        BehaviorTree::ForceSuccess(
            action<GoToLookAt>()
                    ->setPathPlanner(new PointToPoint())
                    ->setGoal([this]() {
                        Vec2 playerPos = world().playerPositionVec2(player());
                        Vec2 ballPos = world().ballPositionVec2();
                        Vec2 playerToBall = ballPos - playerPos;
                        Vec2 final = ballPos + playerToBall.normalized() * Const::Skills::Dribbling::max_push_distance;
                    
                        return final;
                    })
                    ->setLookAt([this]() { return aiming().getEnemyGoalKickPosition(player()); })
        )
    });

    setRoot(root);
}