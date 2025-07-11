#include "preciseshoottogoal.hh"

#include "algorithm/pathplanner/pflorinho/pflorinho.hh"
#include "constants/constants.hh"

PreciseShootToGoal::PreciseShootToGoal(const PlayerID &playerId, SSLController &controller, const World &worldRef)
    : SSLBehavior(playerId, controller, worldRef, "PreciseShootToGoal") {
    auto root = BehaviorTree::Sequence(
        "Precise Shooting Sequence", {
            BehaviorTree::Fallback("PositionToShootFallback", {
            BehaviorTree::Condition("IsBehindBall",
                                    [this]() {
                                        return ballInteraction().isBehindBall(
                                            world().playerPositionVec2(player()),
                                            aiming().getEnemyGoalKickPosition(player()), 0.105f);
                                    }),
            BehaviorTree::Condition("IsAimingAtGoal", [this]() {
                return aiming().isAimingAtPosition(player(), aiming().getEnemyGoalKickPosition(player()));
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
                                                        1.0f);
                }),
            }),
        action<Kick>()->setPower(10.f)});

    setRoot(root);
}