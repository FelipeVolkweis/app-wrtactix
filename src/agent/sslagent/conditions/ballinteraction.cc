#include "constants/constants.hh"
#include "types/angle.hh"

#include "ballinteraction.hh"
#include <limits>

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
    float ori = world_.playerOrientation(playerId).value();
    Vec2 kickingDevicePos = Vec2(cosf(ori), sinf(ori)).normalized() * Const::Physics::kicking_device_distance + playerPos;

    Vec2 ballToPlayer = kickingDevicePos - ballPos;
    auto proj = ballToPlayer.dot(ballVel) * ballVel / ballVel.squaredNorm();

    return ballPos + proj;
}

bool BallInteraction::isClosestToBallByMargin(const PlayerID &playerId, float margin, Colors::Color color) const {
    const auto ballPos = world_.ballPositionVec2();

    const auto it = std::find_if(world_.availablePlayers(color).begin(), world_.availablePlayers(color).end(),
                                 [&](const PlayerID &p) { return p == playerId; });
    if (it == world_.availablePlayers(color).end()) {
        return false;
    }
    float targetDist = (ballPos - world_.playerPositionVec2(playerId)).norm();

    for (const auto &p : world_.availablePlayers(color)) {
        if (p == playerId)
            continue;
        float d = (ballPos - world_.playerPositionVec2(p)).norm();
        if (targetDist + margin >= d) {
            return false;
        }
    }

    return true;
}

bool BallInteraction::isOurClosestToBallByMargin(const PlayerID &playerId, float margin) const {
    return isClosestToBallByMargin(playerId, margin, world_.ourColor());
}

bool BallInteraction::isTheirsClosestToBallByMargin(const PlayerID &playerId, float margin) const {
    return isClosestToBallByMargin(playerId, margin, world_.theirColor());
}