#include "algorithm/pathplanner/pflorinho/pflorinho.hh"
#include "constants/constants.hh"

#include "shoottogoal.hh"

ShootToGoal::ShootToGoal(const PlayerID &playerId, SSLController &controller, const World &worldRef)
    : SSLBehavior(playerId, controller, worldRef, "ShootToGoal") {
    auto root = BehaviorTree::Sequence(
        "Shooting Sequence", {
            BehaviorTree::Fallback(
             "PositionToShootFallback", {
                BehaviorTree::Condition("IsBehindBall",
                                        [this]() {
                                            return ballInteraction().isBehindBall(
                                                world().playerPositionVec2(player()),
                                                aiming().getEnemyGoalKickPosition(player()), 0.105f);
                                        }),
                action<GoToLookAt>()
                    ->setPathPlanner(new PFLorinho())
                    ->setGoal([this]() {
                        return ballInteraction().behindBall(aiming().getEnemyGoalKickPosition(player()), Const::Skills::Kicking::optimal_distance);
                    })
                    ->setLookAt([this]() { return aiming().getEnemyGoalKickPosition(player()); })
                    ->setWaypoint([this]() {
                        return ballInteraction().behindBall(aiming().getEnemyGoalKickPosition(player()), 2 * Const::Skills::Kicking::optimal_distance);
                    })
                    ->avoidTeammates()
                    ->avoidOpponents()
                    ->avoidTheirGoal()
                    ->avoidBall([this]() {
                        return !ballInteraction().isBehindBall(world().playerPositionVec2(player()),
                                                            aiming().getEnemyGoalKickPosition(player()), 0.3f,
                                                            0.7f);
                    }),
                // action<RotateAround>()
                //     ->setLookAt([this]() {
                //         return aiming().getEnemyGoalKickPosition(player());
                //     })
                //     ->setPoint([this]() {
                //         return world().ballPositionVec2();
                //     })
                //     ->setRadius(0.15f)
             }),
         action<Kick>()->setPower(10.f)});

    setRoot(root);
}