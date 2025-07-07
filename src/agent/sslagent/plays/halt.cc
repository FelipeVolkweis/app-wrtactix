#include "agent/sslagent/behaviors/behaviors.hh"

#include "halt.hh"

SSLBehavior *PlayHalt::getBehavior(const SSLRole &role, const PlayerID &player, SSLController &controller) {
    return new DoNothing(player, controller, world_);
}