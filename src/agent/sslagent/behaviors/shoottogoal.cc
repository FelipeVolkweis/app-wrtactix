#include "agent/sslagent/actions/actions.hh"
#include "algorithm/behaviortree/behaviortree.hh"
#include "algorithm/pathplanner/astar/astar.hh"

#include "shoottogoal.hh"

ShootToGoal::ShootToGoal(const PlayerID &playerId, SSLController &controller, const World &worldRef)
    : SSLBehavior(playerId, controller, worldRef, "ShootToGoal") {
    auto root = BehaviorTree::Fallback(
        "Shooting Sequence",
        {
            BehaviorTree::Condition("Is Behing Ball",
                                    [this]() {
                                        return ballInteraction().isBehindBall(world().playerPositionVec2(player()),
                                                                              world().ballPositionVec2(), 0.5f);
                                    }),
            action<GoToLookAt>()
                ->setPathPlanner(new AStar())
                ->setGoal([this]() { return ballInteraction().behindBall(world().playerPositionVec2(player()), 0.5f); })
                ->setLookAt([this]() { return world().ballPositionVec2(); }),
        });

    rootNode_ = root;
}