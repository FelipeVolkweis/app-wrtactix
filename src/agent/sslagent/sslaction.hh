#ifndef SSLACTION_HH
#define SSLACTION_HH

#include "algorithm/behaviortree/base/execution/action.hh"
#include "algorithm/pathplanner/pathplanner.hh"
#include "types/playerid.hh"
#include "types/vec2.hh"
#include "world/world.hh"

#include "sslcontroller.hh"

using namespace WRBeT;

class SSLAction : public Action {
public:
    SSLAction(const PlayerID &player, SSLController &controller, const World &world, const QString &name)
        : player_(player), controller_(controller), world_(world), Action(name) {}

protected:
    const PlayerID &id() const {
        return player_;
    }

    SSLController &controller() {
        return controller_;
    }

    const World &world() const {
        return world_;
    }

private:
    PlayerID player_;
    SSLController &controller_;
    const World &world_;
};

#endif