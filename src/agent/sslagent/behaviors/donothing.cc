#include "agent/sslagent/actions/actions.hh"

#include "donothing.hh"

DoNothing::DoNothing(const PlayerID &player, SSLController &controller, const World &world)
    : SSLBehavior(player, controller, world, "DoNothing") {
    auto root = action<Idle>();
    rootNode_ = root;
}