#include "algorithm/behaviortree/behaviortree.hh"

#include "shoottogoal.hh"

ShootToGoal::ShootToGoal(const PlayerID &player, SSLController &controller, const World &world)
    : SSLBehavior(player, controller, world, "ShootToGoal") {
    auto root = BehaviorTree::Sequence("Shooting Sequence", {

                                                            });
}