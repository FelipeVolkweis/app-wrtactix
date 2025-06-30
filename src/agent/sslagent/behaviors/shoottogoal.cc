#include "agent/sslagent/actions/actions.hh"
#include "algorithm/behaviortree/behaviortree.hh"
#include "algorithm/pathplanner/astar/astar.hh"
#include "algorithm/pathplanner/potentialfield/potentialfield.hh"

#include "shoottogoal.hh"

ShootToGoal::ShootToGoal(const PlayerID &playerId, SSLController &controller, const World &worldRef)
    : SSLBehavior(playerId, controller, worldRef, "ShootToGoal") {
    float katt = 1.0;
    float krep = 0.01;
    float minRad = 1.5f;
    float threshhold = 0.25;
    float epsilon = 0.1;
    auto root = BehaviorTree::Fallback("Shooting Sequence",
        {
            BehaviorTree::Condition("Is Behing Ball",
                                    [this]() {
                                        return ballInteraction().isBehindBall(world().playerPositionVec2(player()),
                                                                              world().ballPositionVec2(), 0.1f);
                                    }),
            action<GoToLookAt>()
                ->setPathPlanner(new PotentialField(katt, krep, minRad, threshhold, epsilon))
                ->setGoal([this]() { return ballInteraction().behindBall(world().playerPositionVec2(player()), 0.1f); })
                ->setLookAt([this]() { return world().ballPositionVec2(); })
                ->avoidTeammates()
                ->avoidOpponents(),
        });

    rootNode_ = root;
}