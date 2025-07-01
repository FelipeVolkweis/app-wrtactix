#include "agent/sslagent/actions/actions.hh"
#include "algorithm/behaviortree/behaviortree.hh"
#include "algorithm/pathplanner/astar/astar.hh"
#include "algorithm/pathplanner/pointtopoint/pointtopoint.hh"
#include "algorithm/pathplanner/potentialfield/potentialfield.hh"
#include "algorithm/pathplanner/starpotential/starpotential.hh"
#include "constants/constants.hh"

#include "shoottogoal.hh"

ShootToGoal::ShootToGoal(const PlayerID &playerId, SSLController &controller, const World &worldRef)
    : SSLBehavior(playerId, controller, worldRef, "ShootToGoal") {
    auto root = BehaviorTree::Fallback(
        "Shooting Sequence",
        {
            BehaviorTree::Condition("Is Behing Ball",
                                    [this]() {
                                        return ballInteraction().isBehindBall(
                                            world().playerPositionVec2(player()),
                                            aiming().getEnemyGoalKickPosition(player()), 0.1f);
                                    }),
            action<GoToLookAt>()
                // ->setPathPlanner(new PotentialField(
                //     Const::PathPlanner::PotentialField::katt, Const::PathPlanner::PotentialField::krep,
                //     Const::PathPlanner::PotentialField::min_rad, Const::PathPlanner::PotentialField::threshold,
                //     Const::PathPlanner::PotentialField::epsilon))
                // ->setPathPlanner(new AStar())
                // ->setPathPlanner(new PointToPoint())
                ->setPathPlanner(new StarPotential(
                    Const::PathPlanner::PotentialField::katt, Const::PathPlanner::PotentialField::krep,
                    Const::PathPlanner::PotentialField::min_rad, Const::PathPlanner::StarPotential::threshold,
                    Const::PathPlanner::PotentialField::epsilon))
                ->setGoal([this]() {
                    return ballInteraction().behindBall(aiming().getEnemyGoalKickPosition(player()), 0.2f);
                })
                ->setLookAt([this]() { return aiming().getEnemyGoalKickPosition(player()); })
                ->avoidTeammates()
                ->avoidOpponents()
                ->avoidTheirGoal(),
        });

    rootNode_ = root;
}