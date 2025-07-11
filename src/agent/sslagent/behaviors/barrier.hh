#ifndef BARRIER_HH
#define BARRIER_HH

#include "agent/sslagent/sslbehavior.hh"

class BarrierMiddle : public SSLBehavior {
public:
    BarrierMiddle(const PlayerID &player, SSLController &controller, const World &world);

};

class BarrierLeft : public SSLBehavior {
public:
    BarrierLeft(const PlayerID &player, SSLController &controller, const World &world);

};

class BarrierRight : public SSLBehavior {
public:
    BarrierRight(const PlayerID &player, SSLController &controller, const World &world);

};
#endif