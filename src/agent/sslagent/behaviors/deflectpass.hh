#ifndef DEFLECTPASS_HH
#define DEFLECTPASS_HH

#include "agent/sslagent/sslbehavior.hh"

class DeflectPass : public SSLBehavior {
public:
    DeflectPass(const PlayerID &playerId, SSLController &controller, const World &worldRef);

};

#endif