#ifndef SSLPLAY_HH
#define SSLPLAY_HH

#include "algorithm/behaviortree/behaviortree.hh"
#include "types/types.hh"
#include "world/world.hh"

#include "sslcontroller.hh"
#include "sslrole.hh"

class SSLPlay {
public:
    SSLPlay(const World &worldRef) : world_(worldRef) {}

    virtual WRBeT::Node *getBehavior(const SSLRole &role, const PlayerID &player, SSLController &controller) = 0;

private:
    const World &world_;
};

#endif