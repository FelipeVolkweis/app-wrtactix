#ifndef SSLAGENT_HH
#define SSLAGENT_HH

#include <QQueue>

#include "algorithm/behaviortree/base/node.hh"
#include "referee/referee.hh"
#include "types/playerid.hh"
#include "world/world.hh"

#include "sslcontroller.hh"
#include "sslplay.hh"
#include "sslrole.hh"

class SSLAgent {
public:
    SSLAgent(PlayerID id, Sides::Side side, GEARSystem::Controller &controller, const World &world,
             const Referee &referee);

    void observe();
    void listen();
    void think();
    void act();

    bool isActive() const;

    const PlayerID &id() const {
        return id_;
    }

    void setPlay(SSLPlay *play);
    void setRole(SSLRole *role);

private:
    PlayerID id_;
    const World &world_;
    SSLController sslController_;
    const Referee &referee_;

    SSLPlay *play_;
    SSLRole *role_;

    SSLBehavior *currentBehavior_;

    SSLBehavior *calibrateLinear_;
    SSLBehavior *calibrateAngular_;
};

#endif