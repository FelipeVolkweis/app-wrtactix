#ifndef RECEIVEPASS_HH
#define RECEIVEPASS_HH

#include "agent/sslagent/sslbehavior.hh"

class ReceivePass : public SSLBehavior {
public:
    ReceivePass(const PlayerID &playerId, SSLController &controller, const World &worldRef);

};

#endif // RECEIVEPASS_HH