#ifndef IDLE_HH
#define IDLE_HH

#include "agent/sslagent/sslaction.hh"

class Idle : public SSLAction {
public:
    Idle(const PlayerID &player, SSLController &controller, const World &world);

    Status execute();
};

#endif