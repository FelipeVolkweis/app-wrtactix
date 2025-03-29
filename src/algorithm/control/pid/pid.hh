#ifndef PID_HH
#define PID_HH

#include <QQueue>

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

    QQueue<float> errors_;

    float lastError_;
};

#endif