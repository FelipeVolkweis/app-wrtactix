#include "algorithm/geometry/radialsweep/radialsweep.hh"
#include "constants/constants.hh"

#include "keeper.hh"
#include "locations.hh"

Keeper::Keeper(const World &world) : world_(world) {}

Vec2 Keeper::getGoaliePosition() const {
    auto l = getBallImpactLine();
    float mfinal = l.m;
    float bfinal = l.b;

    return getGoaliePositionInCircumference(mfinal, bfinal);
}

Keeper::Line Keeper::getBallImpactLine() const {
    Line lfinal;
    auto ball = world_.ballPositionVec2();

    const Velocity &ballVelocity = world_.ballVelocity();

    float goalCenterX =
        world_.ourSide() == Sides::LEFT ? world_.leftGoal().leftPost().x() : world_.rightGoal().leftPost().x();
    float goalCenterY = 0;

    Vec2 goalCenter(goalCenterX, goalCenterY);

    float angle = TwoD::angleBetweenVectors(goalCenter, ball);

    const float epsilon = 1e-5;
    if (fabs(cos(angle)) < epsilon) {
        lfinal.vertical = true;
        lfinal.x0 = ball.x();
    } else {
        lfinal.vertical = false;
        lfinal.m = tanf(angle);
        lfinal.b = ball.y() - lfinal.m * ball.x();
    }

    auto attackerId = world_.theirClosestPlayerToBall();

    if (attackerId.isValid()) {
        auto attacker = world_.playerPositionVec2(attackerId);

        if (TwoD::distance(attacker, ball) < 0.2f) {
            const Angle &attackerOrientation = world_.playerOrientation(attackerId);
            float m = tanf(attackerOrientation.value());
            float b = attacker.y() - m * attacker.x();

            if (hitsOurGoal(m, b)) {
                lfinal.m = m;
                lfinal.b = b;
                lfinal.vertical = false;
            }
        }
    }

    if (ballVelocity.abs() >= 0.1) {
        float m = ballVelocity.y() / ballVelocity.x();
        float b = ball.y() - m * ball.x();

        if (hitsOurGoal(m, b)) {
            lfinal.m = m;
            lfinal.b = b;
            lfinal.vertical = false;
        }
    }

    return lfinal;
}

QPair<Vec2, float> Keeper::getGoalieCircumference() const {
    const auto &w = world_;
    if (w.ourSide() == Sides::LEFT) {
        Vec2 p1 = TwoD::positionToVector(w.leftGoal().leftPost());
        Vec2 p2 = TwoD::positionToVector(w.leftGoal().rightPost());
        Vec2 p3(w.leftGoal().leftPost().x() + w.leftGoal().getAreaWidth() / 2, 0);

        return TwoD::findCircleFromThreePoints(p1, p2, p3);
    } else {
        Vec2 p1 = TwoD::positionToVector(w.rightGoal().leftPost());
        Vec2 p2 = TwoD::positionToVector(w.rightGoal().rightPost());
        Vec2 p3(w.rightGoal().leftPost().x() - w.rightGoal().getAreaWidth() / 2, 0);

        return TwoD::findCircleFromThreePoints(p1, p2, p3);
    }
}

Vec2 Keeper::getGoaliePositionInCircumference(float m, float b) const {
    auto circumference = getGoalieCircumference();
    auto intersections = TwoD::findLineCircleIntersections(circumference.first, circumference.second, m, b);

    if (intersections.size() == 2) { // Pegar a mais próxima do observador quando tiver + de 1 intersec
        Vec2 qtObserver = world_.ballPositionVec2();

        Position pos1 = TwoD::vectorToPosition(intersections[0]);
        Position pos2 = TwoD::vectorToPosition(intersections[1]);
        Locations loc(world_);
        if (loc.isInTheField(pos1)) {
            return TwoD::positionToVector(pos1);
        }
        if (loc.isInTheField(pos2)) {
            return TwoD::positionToVector(pos2);
        }
    } else if (intersections.size() == 1) { // Tangente
        return intersections[0];
    }
    // qCCritical(DEFENSIVE_EVALUATOR) << "No intersection found";
    if (circumference.first.x() < 0) {
        return Vec2(world_.leftGoal().leftPost().x(), 0);
    } else {
        return Vec2(world_.rightGoal().leftPost().x(), 0);
    }
}

bool Keeper::hitsOurGoal(float m, float b) const {
    const auto &w = world_;
    if (m != m || b != b) { // NaN
        return false;
    }
    if (w.ourSide() == Sides::LEFT) {
        return m * w.leftGoal().leftPost().x() + b < w.leftGoal().getAreaLength() / 2 &&
               m * w.leftGoal().rightPost().x() + b > -w.leftGoal().getAreaLength() / 2;
    } else {
        return m * w.rightGoal().leftPost().x() + b > -w.rightGoal().getAreaLength() / 2 &&
               m * w.rightGoal().rightPost().x() + b < w.rightGoal().getAreaLength() / 2;
    }
}

Vec2 Keeper::getKickOutOfOurArea(const PlayerID &callerId) const {
    // begin Preparing FANA call
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
        Position topAreaLine(true, -3.5, 1, 0);
        Position bottomAreaLine(true, -3.5, -1, 0);
        minInterval = TwoD::angleBetweenPositions(world_.ballPosition(), bottomAreaLine);
        maxInterval = TwoD::angleBetweenPositions(world_.ballPosition(), topAreaLine);
        r = qMax(TwoD::distance(world_.ballPosition(), topAreaLine),
                 TwoD::distance(world_.ballPosition(), bottomAreaLine));
    } else {
        Position topAreaLine(true, 3.5, 1, 0);
        Position bottomAreaLine(true, 3.5, -1, 0);
        minInterval = TwoD::angleBetweenPositions(world_.ballPosition(), topAreaLine);
        maxInterval = TwoD::angleBetweenPositions(world_.ballPosition(), bottomAreaLine);
        r = qMax(TwoD::distance(world_.ballPosition(), bottomAreaLine),
                 TwoD::distance(world_.ballPosition(), topAreaLine));
    }

    // end
    RadialSweep radialSweep(observer, obstacles, Const::Physics::robot_radius, {minInterval, maxInterval}, r);
    auto freeAngles = radialSweep.getFreeAngles();

    if (freeAngles.size() == 0) {
        return Vec2(0, 3);
    }

    auto largestInterval = RadialSweep::getLargestAngleInterval(freeAngles);
    auto centerAngle = RadialSweep::getCenterOfInterval(largestInterval);

    // y = mx + b
    float m = tan(centerAngle.radians());
    float b = observer.y() - m * observer.x();

    float x = 0;
    float y = m * x + b;

    return Vec2(x, y);
}