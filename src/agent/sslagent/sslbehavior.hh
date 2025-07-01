#ifndef SSLBEHAVIOR_HH
#define SSLBEHAVIOR_HH

#include "agent/sslagent/conditions/aiming.hh"
#include "agent/sslagent/conditions/ballinteraction.hh"
#include "algorithm/behaviortree/behaviortree.hh"
#include "types/playerid.hh"
#include "world/world.hh"

#include "sslcontroller.hh"

using namespace WRBeT;

class SSLBehavior {
public:
    SSLBehavior(const PlayerID &playerId, SSLController &controller, const World &worldRef, const QString &name)
        : playerId_(playerId), controller_(controller), world_(worldRef), ballInteraction_(world_), aiming_(world_) {}

    template <typename ActionType, typename... Args> ActionType *action(Args &&...args) {
        return new ActionType(playerId_, controller_, world_, std::forward<Args>(args)...);
    }

    const BallInteraction &ballInteraction() const {
        return ballInteraction_;
    }

    const Aiming &aiming() const {
        return aiming_;
    }

    const PlayerID &player() const {
        return playerId_;
    }

    const World &world() const {
        return world_;
    }

    WRBeT::Node *node() const {
        return rootNode_;
    }

private:
    const PlayerID &playerId_;
    SSLController &controller_;
    const World &world_;

    const BallInteraction ballInteraction_;
    const Aiming aiming_;

protected:
    WRBeT::Node *rootNode_ = nullptr;
};

#endif