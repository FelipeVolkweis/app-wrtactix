#include "kick.hh"

Kick::Kick(const PlayerID &player, SSLController &controller, const World &world)
    : SSLAction(player, controller, world, "Kick"), power_(1.0) {}

Kick *Kick::setPower(float power) {
    power_ = power;
    return this;
}

Status Kick::execute() {
    controller().kick(power_);
    return Status::SUCCESS;
}