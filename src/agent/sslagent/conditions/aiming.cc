#include "algorithm/geometry/radialsweep/radialsweep.hh"
#include "constants/constants.hh"

#include "aiming.hh"

Q_LOGGING_CATEGORY(AIMING, "Aiming")

Aiming::Aiming(const World &world) : world_(world) {}

Vec2 Aiming::getEnemyGoalKickPosition(const PlayerID &callerId) const {
    bool trash;
    return getEnemyGoalKickPosition(callerId, trash);
}

Vec2 Aiming::getEnemyGoalKickPosition(const PlayerID &callerId, bool &hasValidOpening) const {
    QVector<Vec2> obstacles;
    const auto &w = world_;
    for (auto p : w.availablePlayers(w.ourColor())) {
        if (p != callerId) {
            obstacles.push_back(w.playerPositionVec2(p));
        }
    }
    for (auto p : w.availablePlayers(w.theirColor())) {
        obstacles.push_back(w.playerPositionVec2(p));
    }

    const Vec2 &observer = w.ballPositionVec2();

    float minInterval, maxInterval;
    float r;

    if (w.ourSide() == Sides::LEFT) {
        minInterval = TwoD::angleBetweenPositions(w.ballPosition(), w.rightGoal().rightPost());
        maxInterval = TwoD::angleBetweenPositions(w.ballPosition(), w.rightGoal().leftPost());
        r = qMax(TwoD::distance(w.ballPosition(), w.rightGoal().rightPost()),
                 TwoD::distance(w.ballPosition(), w.rightGoal().leftPost()));
    } else {
        minInterval = TwoD::angleBetweenPositions(w.ballPosition(), w.leftGoal().rightPost());
        maxInterval = TwoD::angleBetweenPositions(w.ballPosition(), w.leftGoal().leftPost());
        r = qMax(TwoD::distance(w.ballPosition(), w.leftGoal().leftPost()),
                 TwoD::distance(w.ballPosition(), w.leftGoal().rightPost()));
    }

    RadialSweep radialSweep(observer, obstacles, Const::Physics::robot_radius, {minInterval, maxInterval}, r);
    auto freeAngles = radialSweep.getFreeAngles();
    auto largestInterval = RadialSweep::getLargestAngleInterval(freeAngles);

    if (largestInterval.size() < Const::Skills::Kicking::min_angle_to_shoot_to_goal) {
        hasValidOpening = false;
    } else {
        hasValidOpening = true;
    }

    if (freeAngles.isEmpty()) {
        if (w.ourSide() == Sides::LEFT) {
            return Vec2(w.rightGoal().leftPost().x(), 0);
        } else {
            return Vec2(w.leftGoal().leftPost().x(), 0);
        }
    }

    auto centerAngle = RadialSweep::getCenterOfInterval(largestInterval);

    float m = tan(centerAngle.radians());
    float b = observer.y() - m * observer.x();

    float x = w.ourSide() == Sides::LEFT ? w.rightGoal().leftPost().x() : w.leftGoal().leftPost().x();
    float y = m * x + b;

    return Vec2(x, y);
}

bool Aiming::isAimingAtPosition(const PlayerID &player, const Vec2 &aimTarget) const {
    auto pos = world_.playerPositionVec2(player);
    auto ori = world_.playerOrientation(player);
    float angle = TwoD::angleBetweenVectors(pos, aimTarget);
    float diff = fabs(TwoD::angleDiff(ori.value(), angle));

    return diff < Const::AI::angle_tolerance;
}

Vec2 Aiming::getPassTarget(const PlayerID &player, const PlayerID &receiver) const {
    Vec2 target = world_.playerPositionVec2(receiver);
    Vec2 playerPos = world_.playerPositionVec2(player);
    Vec2 dir = target - playerPos;
    dir.normalize();

    return target + dir * Const::Physics::kicking_device_distance;
}

Vec2 Aiming::getPassTarget(const PlayerID &player) const {
    return getPassTarget(player, getBestReceiver());
}

PlayerID Aiming::getBestReceiver() const {
    return PlayerID(0, 1);
}

bool Aiming::canTheReceiverTrapTheBall(const PlayerID &player, const PlayerID &receiver) const {
    float cosineThreshold = -0.9f;
    Vec2 receiverPos = world_.playerPositionVec2(receiver);
    Vec2 playerPos = world_.playerPositionVec2(player);
    Vec2 ballPos = world_.ballPositionVec2();

    Vec2 receiverToBallDir = (ballPos - receiverPos).normalized();
    Vec2 playerToBallDir = (ballPos - playerPos).normalized();

    auto cosine = receiverToBallDir.dot(playerToBallDir) / (receiverToBallDir.norm() * playerToBallDir.norm());
    if (cosine > cosineThreshold) {
        return false;
    }

    float rcvOri = world_.playerOrientation(receiver).value();
    Vec2 receiverOrientation = Vec2(cosf(rcvOri), sinf(rcvOri));
    cosine = receiverOrientation.dot(playerToBallDir) / (receiverOrientation.norm() * playerToBallDir.norm());
    if (cosine > cosineThreshold) {
        return false;
    }

    return true;
}