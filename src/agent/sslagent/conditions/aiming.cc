#include "algorithm/geometry/radialsweep/radialsweep.hh"
#include "constants/constants.hh"
#include <algorithm>

#include "aiming.hh"

Q_LOGGING_CATEGORY(AIMING, "Aiming")

Aiming::Aiming(const World &world) : world_(world) {}

Vec2 Aiming::getEnemyGoalKickPosition(const PlayerID &callerId) const {
    bool trash;
    return getEnemyGoalKickPosition(callerId, trash);
}

RadialSweep Aiming::getEnemyGoalSweep(const PlayerID &callerId, const Vec2 &observer) const {
    QVector<Vec2> obstacles;
    const auto &pos = TwoD::vectorToPosition(observer);
    const auto &w = world_;
    for (auto p : w.availablePlayers(w.ourColor())) {
        if (p != callerId) {
            obstacles.push_back(w.playerPositionVec2(p));
        }
    }
    for (auto p : w.availablePlayers(w.theirColor())) {
        obstacles.push_back(w.playerPositionVec2(p));
    }

    float minInterval, maxInterval;
    float r;

    if (w.ourSide() == Sides::LEFT) {
        minInterval = TwoD::angleBetweenPositions(pos, w.rightGoal().rightPost());
        maxInterval = TwoD::angleBetweenPositions(pos, w.rightGoal().leftPost());
        r = qMax(TwoD::distance(pos, w.rightGoal().rightPost()),
                 TwoD::distance(pos, w.rightGoal().leftPost()));
    } else {
        minInterval = TwoD::angleBetweenPositions(pos, w.leftGoal().rightPost());
        maxInterval = TwoD::angleBetweenPositions(pos, w.leftGoal().leftPost());
        r = qMax(TwoD::distance(pos, w.leftGoal().leftPost()),
                 TwoD::distance(pos, w.leftGoal().rightPost()));
    }

    RadialSweep radialSweep(observer, obstacles, Const::Physics::robot_radius, {minInterval, maxInterval}, r);
    return radialSweep;
}

Vec2 Aiming::getEnemyGoalKickPosition(const PlayerID &callerId, bool &hasValidOpening) const {
    const auto &w = world_;
    const Vec2 &observer = w.ballPositionVec2();

    auto radialSweep = getEnemyGoalSweep(callerId, observer);
    auto freeAngles = radialSweep.getFreeAngles();
    auto largestInterval = RadialSweep::getLargestAngleInterval(freeAngles);

    if (largestInterval.size() < Const::Skills::Kicking::min_angle_to_shoot_to_goal) {
        hasValidOpening = false;
    } else {
        hasValidOpening = true;
    }

    if (freeAngles.isEmpty()) {
        return theirMiddleGoal();
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
    float cosineThreshold = cosf(Const::Skills::Kicking::min_angle_to_pass);
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

Vec2 Aiming::getEnemyGoalDeflectPosition(const PlayerID &callerId, bool &hasValidOpening) const {
    const auto &w = world_;
    auto player = w.playerPositionVec2(callerId);

    auto ballDir = w.playerPositionVec2(callerId) - w.ballPositionVec2();

    auto radialSweep = getEnemyGoalSweep(callerId, player);
    auto intervals = radialSweep.getFreeAngles();

    QVector<AngleEvent> events;
    
    WRAngle angle(atan2f(ballDir.y(), ballDir.x()));
    float alpha = Const::Skills::Kicking::deflect_angle;

    WRAngle a1 = angle - WRAngle(alpha);
    WRAngle a2 = angle + WRAngle(alpha);

    if (a1.quadrant() == Quadrant::IV &&
            a2.quadrant() == Quadrant::I) { // Checks if the obstacle is in the border of 0, 2pi
            events.push_back({0, true});
            events.push_back({M_2_PI_EXCLUSIVE, false});
        }

        events.push_back({a1, true});
        events.push_back({a2, false});

    std::sort(events.begin(), events.end(), [](const AngleEvent &a, const AngleEvent &b) { return a.angle < b.angle; });

    QVector<AngleInterval> deflectAngles;
    for (int i = 0; i < events.size() / 2; i++) {
        deflectAngles.push_back(AngleInterval(events[i].angle, events[i + 1].angle));
    }

    QVector<AngleInterval> validDeflectAngles;
    for (auto &defAngle : deflectAngles) {
        for (auto &freeAngle : intervals) {
            auto intersection = freeAngle.getIntervalIntersection(defAngle);
            if (intersection.size() > Const::Skills::Kicking::min_angle_to_deflect) {
                validDeflectAngles.push_back(intersection);
                hasValidOpening = true;
            }
        }
    }

    if (validDeflectAngles.isEmpty() || !hasValidOpening) {
        return theirMiddleGoal();
    }

    auto largestInterval = RadialSweep::getLargestAngleInterval(validDeflectAngles);
    auto centerAngle = RadialSweep::getCenterOfInterval(largestInterval);
    float m = tan(centerAngle.radians());
    float b = player.y() - m * player.x();

    float x = w.ourSide() == Sides::LEFT ? w.rightGoal().leftPost().x() : w.leftGoal().leftPost().x();
    float y = m * x + b;
    hasValidOpening = true;
    return Vec2(x, y);
}

Vec2 Aiming::getEnemyGoalDeflectPosition(const PlayerID &callerId) const { 
    bool trash;
    return getEnemyGoalDeflectPosition(callerId, trash);
}

Vec2 Aiming::theirMiddleGoal() const {
    const auto &w = world_;

    if (w.ourSide() == Sides::LEFT) {
        return Vec2(w.rightGoal().leftPost().x(), 0);
    } 
    return Vec2(w.leftGoal().leftPost().x(), 0);
}