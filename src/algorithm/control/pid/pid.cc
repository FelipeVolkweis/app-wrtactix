#include "pid.hh"
#include <algorithm>

PID::PID(float kp, float ki, float kd, float dt, float max)
    : kp_(kp), ki_(ki), kd_(kd), dt_(dt), max_(max), lastError_(0), integral_(0) {}

float PID::computeSignal(float error) {
    integral_ = std::clamp(integral_ + (error * dt_), -max_, max_);

    float p = kp_ * error;
    float i = ki_ * integral_;
    float d = kd_ * (error - lastError_) / dt_;

    if (firstRun_) {
        d = 0;
        firstRun_ = false;
    }

    lastError_ = error;

    float signal = p + i + d;
    if (signal > max_) {
        signal = max_;
    } else if (signal < -max_) {
        signal = -max_;
    }

    return signal;
}

void PID::reset() {
    lastError_ = 0;
    integral_ = 0;
}