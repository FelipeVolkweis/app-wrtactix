#ifndef AIMING_HH
#define AIMING_HH

#include "world/world.hh"

class Aiming {
public:
    Aiming(const World &world);

    Vec2 getEnemyGoalKickPosition(const PlayerID &callerId) const;

private:
    const World &world_;
};

#endif