#ifndef SHOOTTOGOAL_HH
#define SHOOTTOGOAL_HH

#include "agent/sslagent/sslbehavior.hh"

class ShootToGoal : public SSLBehavior {
public:
    ShootToGoal(const PlayerID &player, SSLController &controller, const World &world);

private:
};

#endif