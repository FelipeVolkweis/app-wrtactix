#include "pass.hh"

#include "algorithm/pathplanner/pflorinho/pflorinho.hh"
#include "constants/constants.hh"

Pass::Pass(const PlayerID &playerId, SSLController &controller, const World &worldRef)
    : SSLBehavior(playerId, controller, worldRef, "Pass") {
    auto root = BehaviorTree::Sequence(
        "PassingSequence", {
            BehaviorTree::Fallback(
             "PositionToShootFallback", {
                BehaviorTree::Condition("IsBehindBall",
                                        [this]() {
                                            return ballInteraction().isBehindBall(
                                                world().playerPositionVec2(player()),
                                                aiming().getPassTarget(player()), 0.105f);
                                        }),
                action<GoToLookAt>()
                    ->setPathPlanner(new PFLorinho())
                    ->setGoal([this]() {
                        return ballInteraction().behindBall(aiming().getPassTarget(player()), Const::Skills::Kicking::optimal_distance);
                    })
                    ->setLookAt([this]() { return aiming().getPassTarget(player()); })
                    ->avoidTeammates()
                    ->avoidOpponents()
                    ->avoidTheirGoal()
                    ->avoidBall([this]() {
                        return !ballInteraction().isBehindBall(world().playerPositionVec2(player()),
                                                            aiming().getPassTarget(player()), 0.4f,
                                                            1.0f);
                    }),
             }),
        BehaviorTree::Condition("CanTheReceiverTrapTheBall",
                                        [this]() {
                                            auto receiver = aiming().getBestReceiver();
                                            return aiming().canTheReceiverTrapTheBall(player(), receiver);
                                        }),
        action<Kick>()->setPower(Const::Skills::Kicking::min_velocity)});
    setRoot(root);
}