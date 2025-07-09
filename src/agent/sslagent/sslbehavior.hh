#ifndef SSLBEHAVIOR_HH
#define SSLBEHAVIOR_HH

#include "agent/sslagent/actions/actions.hh"
#include "agent/sslagent/conditions/aiming.hh"
#include "agent/sslagent/conditions/ballinteraction.hh"
#include "agent/sslagent/conditions/keeper.hh"
#include "agent/sslagent/conditions/locations.hh"
#include "algorithm/behaviortree/behaviortree.hh"
#include "types/playerid.hh"
#include "world/world.hh"

#include "sslcontroller.hh"

using namespace WRBeT;

class SSLBehavior {
public:
    SSLBehavior(const PlayerID &playerId, SSLController &controller, const World &worldRef, const QString &name)
        : playerId_(playerId), controller_(controller), world_(worldRef), ballInteraction_(world_), aiming_(world_),
          keeper_(world_), locations_(world_) {}

    template <typename ActionType, typename... Args> ActionType *action(Args &&...args) {
        return new ActionType(playerId_, controller_, world_, std::forward<Args>(args)...);
    }

    const BallInteraction &ballInteraction() const {
        return ballInteraction_;
    }

    const Aiming &aiming() const {
        return aiming_;
    }

    const Keeper &keeper() const {
        return keeper_;
    }

    const Locations &locations() const {
        return locations_;
    }

    const PlayerID &player() const {
        return playerId_;
    }

    const World &world() const {
        return world_;
    }

    WRBeT::Node *root() const {
        return rootNode_;
    }

    void setRoot(WRBeT::Node *node) {
        rootNode_ = node;
    }

private:
    const PlayerID &playerId_;
    SSLController &controller_;
    const World &world_;

    const BallInteraction ballInteraction_;
    const Aiming aiming_;
    const Keeper keeper_;
    const Locations locations_;

protected:
    WRBeT::Node *rootNode_ = nullptr;
};

#endif