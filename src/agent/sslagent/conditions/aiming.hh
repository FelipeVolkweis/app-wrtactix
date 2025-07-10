#ifndef AIMING_HH
#define AIMING_HH

#include "world/world.hh"
#include "algorithm/geometry/radialsweep/radialsweep.hh"

class Aiming {
public:
    Aiming(const World &world);

    Vec2 getEnemyGoalKickPosition(const PlayerID &callerId) const;
    Vec2 getEnemyGoalKickPosition(const PlayerID &callerId, bool &hasValidOpening) const;

    bool isAimingAtPosition(const PlayerID &player, const Vec2 &aimTarget) const;
    Vec2 getPassTarget(const PlayerID &player, const PlayerID &receiver) const;
    Vec2 getPassTarget(const PlayerID &player) const;
    PlayerID getBestReceiver() const;
    bool canTheReceiverTrapTheBall(const PlayerID &player, const PlayerID &receiver) const;

    Vec2 getEnemyGoalDeflectPosition(const PlayerID &callerId, bool &hasValidOpening) const;
    Vec2 getEnemyGoalDeflectPosition(const PlayerID &callerId) const;

private:
    const World &world_;

    RadialSweep getEnemyGoalSweep(const PlayerID &callerId, const Vec2 &observer) const;

    Vec2 theirMiddleGoal() const;
};

#endif