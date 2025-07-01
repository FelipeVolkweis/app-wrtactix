#include "ballinteraction.hh"

BallInteraction::BallInteraction(const World &world) : world_(world) {}

Vec2 BallInteraction::behindBall(const Vec2 &reference, float distance) const {
    Vec2 ballPosition = world_.ballPositionVec2();
    Vec2 direction = (ballPosition - reference).normalized();
    return ballPosition + direction * distance;
}

bool BallInteraction::isBehindBall(const Vec2 &object, const Vec2 &reference, float distance) const {
    Vec2 ballPosition = world_.ballPositionVec2();
    Vec2 direction = (ballPosition - reference).normalized();
    Vec2 targetPosition = ballPosition + direction * distance;
    return (object - targetPosition).norm() < 0.01; // millimetric precision
}