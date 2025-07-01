#ifndef PID_HH
#define PID_HH

#include "algorithm/control/control.hh"

class PID : public Control {
public:
    PID(float kp, float ki, float kd, float dt, float max);
    float computeSignal(float error);
    void reset();

private:
    const float kp_;
    const float ki_;
    const float kd_;
    const float dt_;
    const float max_;

    float integral_;

    float lastError_;

    bool firstRun_ = true;
};

#endif