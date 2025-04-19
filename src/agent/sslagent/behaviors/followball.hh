#ifndef FOLLOWBALL_HH
#define FOLLOWBALL_HH

#include "agent/sslagent/sslbehavior.hh"

class FollowBall : public SSLBehavior {
public:
    FollowBall(const PlayerID &player, SSLController &controller, const World &world);
};

#endif // FOLLOWBALL_HH