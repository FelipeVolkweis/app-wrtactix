#ifndef ATTACKERSUPPORT_HH
#define ATTACKERSUPPORT_HH

#include "agent/sslagent/sslbehavior.hh"

class AttackerSupport : public SSLBehavior {
public:
    AttackerSupport(const PlayerID &playerId, SSLController &controller, const World &worldRef);

private:
};

#endif