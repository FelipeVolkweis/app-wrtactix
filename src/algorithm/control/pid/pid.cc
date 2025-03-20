#include "pid.hh"
#include <limits>

#define MAX_ERRORS 3

PID::PID(float kp, float ki, float kd, float dt, float max)
    : kp_(kp), ki_(ki), kd_(kd), dt_(dt), max_(max), lastError_(0) {}

float PID::computeSignal(float error) {
    errors_.enqueue(error);
    if (errors_.size() > MAX_ERRORS) {
        errors_.dequeue();
    }

    float errorSum = 0;

    for (auto &e : errors_) {
        errorSum += e;
    }

    float p = kp_ * error;
    float i = ki_ * errorSum;
    float d = kd_ * (error - lastError_) / dt_;

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
    errors_.clear();
    lastError_ = 0;
}