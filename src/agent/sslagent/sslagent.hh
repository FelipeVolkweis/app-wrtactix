#ifndef SSLAGENT_HH
#define SSLAGENT_HH

#include <QQueue>

#include "algorithm/behaviortree/base/node.hh"
#include "referee/referee.hh"
#include "types/playerid.hh"
#include "world/world.hh"

#include "sslcontroller.hh"

class SSLAgent {
public:
    SSLAgent(PlayerID id, Sides::Side side, GEARSystem::Controller &controller);

    void observe();
    void listen();
    void think();
    void act();

    bool isActive() const;

    const PlayerID& id() const {
        return id_;
    }

private:
    PlayerID id_;
    World world_;
    SSLController sslController_;
    Referee referee_;

    WRBeT::Node *currentBehavior_;
    WRBeT::Node *goTo_;
    WRBeT::Node *idle_;
};

#endif