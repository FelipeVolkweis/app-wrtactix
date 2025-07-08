#ifndef BARRIER_HH
#define BARRIER_HH

#include "agent/sslagent/sslbehavior.hh"

class Barrier : public SSLBehavior {
public:
    Barrier(const PlayerID &player, SSLController &controller, const World &world);

};
#endif