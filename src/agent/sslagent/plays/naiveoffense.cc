#include "naiveoffense.hh"
#include "agent/sslagent/behaviors/behaviors.hh"

SSLBehavior *PlayNaiveOffense::getBehavior(const SSLRole &role, const PlayerID &player, SSLController &controller) {
    switch (role.getType()) {
    case SSLRoleType::GOALKEEPER:
        return new GoalKeeper(player, controller, world_);
    case SSLRoleType::STRIKER:
        return new ShootToGoal(player, controller, world_);
    default:
        return new DoNothing(player, controller, world_);
        break;
    }
    return nullptr;
}