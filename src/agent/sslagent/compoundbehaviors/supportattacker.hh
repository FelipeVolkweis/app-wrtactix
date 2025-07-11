#ifndef SUPPORTATTACKER_HH
#define SUPPORTATTACKER_HH

#include "agent/sslagent/sslbehavior.hh"

class SupportAttacker : public SSLBehavior {
public:
    SupportAttacker(const PlayerID &playerId, SSLController &controller, const World &worldRef);

private:
};

#endif