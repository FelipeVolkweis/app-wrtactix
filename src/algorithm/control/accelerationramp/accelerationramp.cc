#include "accelerationramp.hh"
#include <algorithm>

AccelerationRamp::AccelerationRamp(float maxAcceleration, float maxSpeed, float dt)
    : maxAcceleration_(maxAcceleration), maxSpeed_(maxSpeed), dt_(dt) {}

Vec2 AccelerationRamp::fitSpeedToRamp(const Vec2 &currentSpeed, const Vec2 &targetSpeed) {
    Vec2 desired = targetSpeed;
    float desiredNorm = desired.norm();
    if (desiredNorm > maxSpeed_) {
        desired = desired.normalized() * maxSpeed_;
    }

    Vec2 dv = desired - currentSpeed;

    float maxDeltaV = maxAcceleration_ * dt_;
    float dvNorm = dv.norm();
    if (dvNorm > maxDeltaV) {
        dv = dv.normalized() * maxDeltaV;
    }

    Vec2 newSpeed = currentSpeed + dv;

    float newNorm = newSpeed.norm();
    if (newNorm > maxSpeed_) {
        newSpeed = newSpeed.normalized() * maxSpeed_;
    }

    return newSpeed;
}