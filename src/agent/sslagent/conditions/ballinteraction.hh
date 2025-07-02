#ifndef BALLINTERACTION_HH
#define BALLINTERACTION_HH

#include "world/world.hh"

class BallInteraction {
public:
    BallInteraction(const World &world);

    Vec2 behindBall(const Vec2 &reference, float distance) const;
    bool isBehindBall(const Vec2 &object, const Vec2 &reference, float distance) const;
    bool isBehindBall(const Vec2 &object, const Vec2 &reference, float distance, float angleTolerance) const;

private:
    const World &world_;
};

#endif // BALLINTERACTION_HH