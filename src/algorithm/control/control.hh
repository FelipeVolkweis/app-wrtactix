#ifndef CONTROL_HH
#define CONTROL_HH

class Control {
public:
    virtual float computeSignal(float error) = 0;
    virtual void reset() = 0;
};

#endif // CONTROL_HH