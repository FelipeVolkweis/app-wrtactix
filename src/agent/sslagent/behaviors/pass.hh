#ifndef PASS_HH
#define PASS_HH

#include "agent/sslagent/sslbehavior.hh"

class Pass : public SSLBehavior {
public:
    Pass(const PlayerID &playerId, SSLController &controller, const World &worldRef);

};

#endif // PASS_HH