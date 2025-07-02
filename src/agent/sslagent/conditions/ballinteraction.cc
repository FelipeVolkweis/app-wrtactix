#include "constants/constants.hh"
#include "types/angle.hh"

#include "ballinteraction.hh"

BallInteraction::BallInteraction(const World &world) : world_(world) {}

Vec2 BallInteraction::behindBall(const Vec2 &reference, float distance) const {
    Vec2 ballPosition = world_.ballPositionVec2();
    Vec2 direction = (ballPosition - reference).normalized();
    return ballPosition + direction * distance;
}

bool BallInteraction::isBehindBall(const Vec2 &object, const Vec2 &reference, float distance,
                                   float angularTolerance) const {
    Vec2 ballPosition = world_.ballPositionVec2();

    if (TwoD::distance(ballPosition, object) > distance) {
        return false;
    }

    float angBet = TwoD::angleBetweenVectors(object, ballPosition);
    float objOri = TwoD::angleBetweenVectors(object, reference);
    bool isBehind = fabs(TwoD::angleDiff(angBet, objOri)) < angularTolerance;

    return isBehind;
}

bool BallInteraction::isBehindBall(const Vec2 &object, const Vec2 &reference, float distance) const {
    return isBehindBall(object, reference, distance, Const::AI::angle_tolerance);
}