#include "algorithm/geometry/radialsweep/radialsweep.hh"
#include "constants/constants.hh"

#include "aiming.hh"

Aiming::Aiming(const World &world) : world_(world) {}

Vec2 Aiming::getEnemyGoalKickPosition(const PlayerID &callerId) const {
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

    if (freeAngles.isEmpty()) {
        if (w.ourSide() == Sides::LEFT) {
            return Vec2(w.rightGoal().leftPost().x(), 0);
        } else {
            return Vec2(w.leftGoal().leftPost().x(), 0);
        }
    }

    auto largestInterval = RadialSweep::getLargestAngleInterval(freeAngles);
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