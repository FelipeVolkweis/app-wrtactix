#ifndef BALLINTERACTION_HH
#define BALLINTERACTION_HH

#include "types/playerid.hh"
#include "world/world.hh"

class BallInteraction {
public:
    BallInteraction(const World &world);

    Vec2 behindBall(const Vec2 &reference, float distance) const;
    bool isBehindBall(const Vec2 &object, const Vec2 &reference, float distance) const;
    bool isBehindBall(const Vec2 &object, const Vec2 &reference, float distance, float angleTolerance) const;
    bool isTheBallInCollisionRoute(const Vec2 &object) const;
    Vec2 getPositionToInterceptMovingBall(const PlayerID &playerId) const;
    bool isClosestToBallByMargin(const PlayerID &playerId, float margin, Colors::Color color) const;
    bool isOurClosestToBallByMargin(const PlayerID &playerId, float margin) const;
    bool isTheirsClosestToBallByMargin(const PlayerID &playerId, float margin) const;

private:
    const World &world_;
};

#endif // BALLINTERACTION_HH