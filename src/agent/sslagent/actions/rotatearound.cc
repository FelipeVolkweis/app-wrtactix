#include "rotatearound.hh"

RotateAround::RotateAround(const PlayerID &player, SSLController &controller, const World &world)
    : SSLAction(player, controller, world, "RotateAround"), radius_(0.0f) {}

RotateAround *RotateAround::setLookAt(std::function<Vec2()> lookAt) {
    lookAt_ = lookAt;
    return this;
}

RotateAround *RotateAround::setPoint(std::function<Vec2()> point) {
    point_ = point;
    return this;
}

RotateAround *RotateAround::setRadius(float r) {
    radius_ = r;
    return this;
}

Status RotateAround::execute() {
    if (!lookAt_) {
        return Status::FAILURE;
    }

    if (!point_) {
        return Status::FAILURE;
    }

    Vec2 lookAt = lookAt_();
    Vec2 point = point_();
    Vec2 origin = world().playerPositionVec2(id());
    float angle = TwoD::angleBetweenVectors(origin, lookAt);

    if (fabsf(angle) < 0.1) {
        return Status::SUCCESS;
    }

    controller().rotateAroundPoint(point, lookAt, radius_);
    
    return Status::RUNNING;
}