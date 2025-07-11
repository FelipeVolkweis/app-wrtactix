#include "goalkeeper.hh"
#include "constants/constants.hh"
#include "algorithm/pathplanner/pflorinho/pflorinho.hh"

GoalKeeper::GoalKeeper(const PlayerID &playerId,
               SSLController &controller,
               const World &worldRef)
  : SSLBehavior(playerId, controller, worldRef, "GoalKeeper")
{
    // TODO: TEM QUE PENSAR EM ALGO NA SITUAÇÂO QUE A BOLA TA ATRAS DO GOLEIRO PRA 
    // ELE NAO EMPURRA E FAZER GOL CONTRA POR ACIDENTE!!!!!
    auto removeBallFromOurArea = BehaviorTree::Sequence(
        "RemoveBallFromOurArea",
        {
            BehaviorTree::Condition("IsBallInOurArea", [this]() {
                return locations().isInOurGoalArea(world().ballPosition());
            }),
            BehaviorTree::Condition("IsBallNotMoving", [this]() {
                return world().ballVelocity().abs() < 0.1f;
            }),
            BehaviorTree::Sequence("Shooting Sequence",
                {BehaviorTree::Fallback(
                    "PositionToShootFallback",
                    {
                        BehaviorTree::Condition("Is Behing Ball",
                                                [this]() {
                                                    return ballInteraction().isBehindBall(
                                                        world().playerPositionVec2(player()),
                                                        block().getKickOutOfOurArea(player()), 0.105f);
                                                }),
                        action<GoToLookAt>()
                            ->setPathPlanner(new PFLorinho())
                            ->setGoal([this]() {
                                return ballInteraction().behindBall(block().getKickOutOfOurArea(player()), 0.1f);
                            })
                            ->setLookAt([this]() { return block().getKickOutOfOurArea(player()); })
                            ->avoidTeammates()
                            ->avoidOpponents()
                            ->avoidTheirGoal()
                            ->avoidBall([this]() {
                                return !ballInteraction().isBehindBall(world().playerPositionVec2(player()),
                                                                        block().getKickOutOfOurArea(player()), 0.4f,
                                                                        1.0f);
                            }),
                    }),
                action<Kick>()->setPower(10.f)})
        }
    );

    auto goToPosition = BehaviorTree::Sequence(
        "GoTo",
        {
            BehaviorTree::Condition("IsBallNotInOurArea", [this]() {
                return !locations().isInOurGoalArea(world().ballPosition());
            }),
            action<GoToLookAt>()
                ->setGoal([this]() {
                    return block().getGoaliePosition();
                })
                ->setLookAt([this]() {
                    return world().ballPositionVec2();
                })
                ->setPathPlanner(new PFLorinho())
        }
    );

    auto root = BehaviorTree::Fallback(
        "Goalie",
        { removeBallFromOurArea, goToPosition }
    );

    setRoot(root);
}
