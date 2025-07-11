#ifndef PRECISESHOOTOGOAL_HH
#define PRECISESHOOTOGOAL_HH

#include "agent/sslagent/sslbehavior.hh"

class PreciseShootToGoal : public SSLBehavior {
public:
    PreciseShootToGoal(const PlayerID &playerId, SSLController &controller, const World &worldRef);

private:
};

#endif 