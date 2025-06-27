#ifndef DONOTHING_HH
#define DONOTHING_HH

#include "agent/sslagent/sslbehavior.hh"

class DoNothing : public SSLBehavior {
public:
    DoNothing(const PlayerID &player, SSLController &controller, const World &world);
};

#endif