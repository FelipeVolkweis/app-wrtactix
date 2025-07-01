#ifndef CALIBRATELINEAR_HH
#define CALIBRATELINEAR_HH

#include "agent/sslagent/sslbehavior.hh"

class CalibrateLinear : public SSLBehavior {
public:
    CalibrateLinear(const PlayerID &playerId, SSLController &controller, const World &worldRef);

private:
    bool beenToA_ = false;
    bool beenToB_ = false;
    bool beenToC_ = false;
    bool beenToD_ = false;
};

#endif