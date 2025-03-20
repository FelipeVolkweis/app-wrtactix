#include "potentialfield.hh"

#define EPS 1e-5

PotentialField::PotentialField(float kAtt, float kRep, float minRadiusForRepulsiveForce, float conicQuadraticThreshold)
    : kAtt_(kAtt), kRep_(kRep), minRadiusForRepulsiveForce_(minRadiusForRepulsiveForce),
      conicQuadraticThreshold_(conicQuadraticThreshold) {}

void PotentialField::addRepulsiveForce(const Vec2 &obstacle) {
    auto distance = (obstacle - origin_).norm() + EPS;
    if (distance > minRadiusForRepulsiveForce_) {
        return;
    }

    auto potentialForce =
        kRep_ * (1 / minRadiusForRepulsiveForce_ - 1 / distance) * 1 / (distance * distance) * (origin_ - obstacle);

    addForce(potentialForce);
}

void PotentialField::addAttractiveForce() {
    auto distance = (goal_ - origin_).norm() + EPS;
    Vec2 potentialForce;
    if (distance <= conicQuadraticThreshold_) { // quadratic
        potentialForce = kAtt_ * (goal_ - origin_);
    } else { // conic
        potentialForce = kAtt_ * (goal_ - origin_).normalized() * conicQuadraticThreshold_;
    }

    addForce(potentialForce);
}

void PotentialField::addForce(const Vec2 &force) {
    resultantForce_ += force;
}

void PotentialField::setGoFromOriginToGoal(const Vec2 &origin, const Vec2 &goal) {
    origin_ = origin;
    goal_ = goal;
}

Vec2 PotentialField::getForce() const {
    return resultantForce_;
}

void PotentialField::reset() {
    resultantForce_ = Vec2(0, 0);
}