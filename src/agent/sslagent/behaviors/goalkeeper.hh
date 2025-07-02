#ifndef GOALKEEPER_HH
#define GOALKEEPER_HH

#include "agent/sslagent/sslbehavior.hh"

class GoalKeeper : public SSLBehavior {
public:
    GoalKeeper(const PlayerID &player, SSLController &controller, const World &world);

};
#endif