#include "types/angle.hh"

#include "lookat.hh"

LookAt::LookAt(const PlayerID &player, SSLController &controller, const World &world)
    : SSLAction(player, controller, world, "LookAt") {}

LookAt &LookAt::setLookAt(std::function<Vec2()> target) {
    target_ = target;
    return *this;
}

Status LookAt::execute() {
    if (!target_) {
        return Status::FAILURE;
    }

    auto target = target_();
    auto origin = world().playerPositionVec2(id());
    QVector<Vec2> emptyPath;
    auto d = (target - origin);
    auto angle = atan2f(d.y(), d.x());
    auto orientation = world().playerOrientation(id()).value();

    if (fabsf(angle - orientation) < 0.1) { // ?
        return Status::SUCCESS;
    }

    controller().move(emptyPath, angle);

    return Status::RUNNING;
}