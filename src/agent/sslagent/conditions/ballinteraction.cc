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

bool BallInteraction::isTheBallInCollisionRoute(const Vec2 &object) const {
    Vec2 ballPosition = world_.ballPositionVec2();
    Vec2 ballToObject = object - ballPosition;

    float cosine = ballToObject.normalized().dot(TwoD::velocityToVector(world_.ballVelocity()).normalized());
    if (world_.ballVelocityVec2().norm() < Const::Physics::minimum_ball_velocity_to_consider_movement) {
        return false;
    }

    if (cosine > 0.9f) {
        return true;
    }

    return false;
}

Vec2 BallInteraction::getPositionToInterceptMovingBall(const PlayerID &playerId) const {
    Vec2 ballPos = world_.ballPositionVec2();
    Vec2 ballVel = world_.ballVelocityVec2();
    Vec2 playerPos = world_.playerPositionVec2(playerId);

    Vec2 ballToPlayer = playerPos - ballPos;
    auto proj = ballToPlayer.dot(ballVel) * ballVel / ballVel.squaredNorm();

    return ballPos + proj;
}   