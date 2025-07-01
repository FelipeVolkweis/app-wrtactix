#ifndef ACCELERATIONRAMP_HH
#define ACCELERATIONRAMP_HH

#include "types/vec2.hh"

class AccelerationRamp {
public:
    AccelerationRamp(float maxAcceleration, float maxSpeed, float dt);
    Vec2 fitSpeedToRamp(const Vec2 &currentSpeed, const Vec2 &targetSpeed);

private:
    float maxAcceleration_;
    float maxSpeed_;
    float dt_;
};

#endif