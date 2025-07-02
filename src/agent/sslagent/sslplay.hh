#ifndef SSLPLAY_HH
#define SSLPLAY_HH

#include "types/types.hh"
#include "world/world.hh"

#include "sslcontroller.hh"
#include "sslrole.hh"
#include "sslbehavior.hh"

class SSLPlay {
public:
    SSLPlay(const World &worldRef) : world_(worldRef) {}

    virtual SSLBehavior *getBehavior(const SSLRole &role, const PlayerID &player, SSLController &controller) = 0;

protected:
    const World &world_;
};

#endif