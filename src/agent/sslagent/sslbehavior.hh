#ifndef SSLBEHAVIOR_HH
#define SSLBEHAVIOR_HH

#include "algorithm/behaviortree/base/costnode.hh"
#include "types//playerid.hh"
#include "world/world.hh"

#include "sslcontroller.hh"

using namespace WRBeT;

class SSLBehavior : public CostNode {
public:
    SSLBehavior(const PlayerID &player, SSLController &controller, const World &world, const QString &name)
        : CostNode(name), player_(player), controller_(controller), world_(world) {}

    template <typename ActionType, typename... Args> ActionType *action(Args &&...args) {
        return new ActionType(player_, controller_, world_, std::forward<Args>(args)...);
    }

private:
    const PlayerID &player_;
    SSLController &controller_;
    const World &world_;
};

#endif