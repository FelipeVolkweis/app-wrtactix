#include "idle.hh"

Idle::Idle(const PlayerID &player, SSLController &controller, const World &world)
    : SSLAction(player, controller, world, "Idle") {}

Status Idle::execute() {
    return Status::SUCCESS;
}