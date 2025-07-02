#ifndef HALT_HH
#define HALT_HH

#include "agent/sslagent/sslplay.hh"

class PlayHalt : public SSLPlay {
public:
    PlayHalt(const World &worldRef) : SSLPlay(worldRef) {}

    SSLBehavior *getBehavior(const SSLRole &role, const PlayerID &player, SSLController &controller);
};

#endif