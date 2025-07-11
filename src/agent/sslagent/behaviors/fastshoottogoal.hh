#ifndef FASTSHOOTTOGOAL_HH
#define FASTSHOOTOTGOAL_HH

#include "agent/sslagent/sslbehavior.hh"

class FastShootToGoal : public SSLBehavior {
public:
    FastShootToGoal(const PlayerID &playerId, SSLController &controller, const World &worldRef);

private:
};


#endif