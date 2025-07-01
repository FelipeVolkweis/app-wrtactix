#ifndef CALIBRATEANGULAR_HH
#define CALIBRATEANGULAR_HH

#include "agent/sslagent/sslbehavior.hh"

class CalibrateAngular : public SSLBehavior {
public:
    CalibrateAngular(const PlayerID &playerId, SSLController &controller, const World &worldRef);

private:
    bool beenToA_ = false;
    bool beenToB_ = false;
    bool beenToC_ = false;
    bool beenToD_ = false;
};

#endif