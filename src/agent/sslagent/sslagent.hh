#ifndef SSLAGENT_HH
#define SSLAGENT_HH

#include <QQueue>

#include "agent/agent.hh"
#include "types/playerid.hh"
#include "world/world.hh"

#include "sslcontroller.hh"

class SSLAgent : Agent {
public:
    SSLAgent(PlayerID id, GEARSystem::Controller &controller, const World &world);

    void observe();
    void listen();
    void think();
    void act();

private:
    PlayerID id_;
    SSLController sslController_;
};

#endif