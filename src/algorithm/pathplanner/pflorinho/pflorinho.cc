/***
 * Warthog Robotics
 * University of Sao Paulo (USP) at Sao Carlos
 * http://www.warthog.sc.usp.br/
 *
 * This file is part of WRCoach project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#include "algorithm/geometry/twod/twod.hh"
#include "constants/constants.hh"
#include "freeangles/freeangles.hh"
#include "freeangles/utils.hh"

#include "pflorinho.hh"
#include <iostream>

#pragma GCC diagnostic ignored "-Wunused-parameter"

#include <QDebug>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(PF_CATEGORY, "PF");

PFLorinho::PFLorinho() {
    reset();
}

void PFLorinho::reset() {
    _resultantForce.setVector(0.0, 0.0);
    _resultantAngle = 0.0;
    _collisionPositions.clear();
    _repulsive.clear();
}

QString PFLorinho::name() {
    return "PF";
}

QVector<Vec2> PFLorinho::findPath(const Vec2 &start, const Vec2 &end, const QVector<Obstacle> &obstacles) {
    _originPos = TwoD::vectorToPosition(start);
    _goalPos = TwoD::vectorToPosition(end);
    reset();

    for (const auto &o : obstacles) {
        if (o.isBall()) {
            addBall(TwoD::vectorToPosition(o.center));
        } else if (o.isRectangle()) {
            float x = std::clamp(_originPos.x(), o.bottomLeft.x(), o.topRight.x());
            float y = std::clamp(_originPos.y(), o.bottomLeft.y(), o.topRight.y());
            Vec2 obstacleSurface = Vec2{x, y};
            addGoalArea(TwoD::vectorToPosition(obstacleSurface));
        } else if (o.isTeammate()) {
            addOwnRobot(TwoD::vectorToPosition(o.center));
        } else if (o.isOpponent()) {
            addEnemyRobot(TwoD::vectorToPosition(o.center));
        } else {
            qCCritical(PF_CATEGORY) << "Unknown obstacle type in PFLorinho::findPath()";
        }
    }

    run();

    QVector<Vec2> path;
    path.append(start);
    Vec2 resultVec(cosf(_resultantAngle), sinf(_resultantAngle));
    Vec2 mid(start + resultVec * 0.01);
    path.append(mid);
    path.append(end);

    return path;
}

void PFLorinho::run() {
    // Reset resultant force
    _resultantForce.setVector(0.0, 0.0);

    // Add attractive force
    addAttractiveForce(getVector(originPos(), goalPos()));

    // Add repulsive forces
    for (int i = 0; i < _repulsive.size(); i++)
        addRepulsiveForce(getVector(_repulsive.at(i).first, originPos()), _repulsive.at(i).second);

    // Calc resultant angle based on resultant force
    float resultantAngle = atan2(_resultantForce.y(), _resultantForce.x());

    // Check collision
    if (isOnCollisionRoute(resultantAngle))
        resultantAngle = handleCollision(resultantAngle);

    // Store result
    _resultantAngle = resultantAngle;
}

void PFLorinho::addBall(const Position &pos) {
    // Add repulsive position
    addRepulsivePosition(pos, Const::PathPlanner::PFLorinho::ball_repulsive);
    addCollisionPosition(pos, true);
}

void PFLorinho::addGoalArea(const Position &pos) {
    // Add repulsive position
    addRepulsivePosition(pos, Const::PathPlanner::PFLorinho::area_repulsive);
    addCollisionPosition(pos);
}

void PFLorinho::addOwnRobot(const Position &pos) {
    // Add robot
    addRobot(pos);
}

void PFLorinho::addEnemyRobot(const Position &pos) {
    // Add robot
    addRobot(pos);
}

void PFLorinho::addRobot(const Position &pos) {
    // Add repulsive position
    addRepulsivePosition(pos, Const::PathPlanner::PFLorinho::repulsive);
    addCollisionPosition(pos);
}

void PFLorinho::addRepulsiveForce(const Vector &v, float k) {
    // Add force
    addForce(applyDistanceFunction(v, k));
}

void PFLorinho::addAttractiveForce(const Vector &v) {
    // Add force
    addForce(v.getUnitary());
}

void PFLorinho::addForce(const Vector &v) {
    // Sum in resultant force
    _resultantForce += v;
}

Vector PFLorinho::applyDistanceFunction(Vector v, float k) {
    const float d = v.getModule();
    const float xshift = Const::PathPlanner::PFLorinho::x_shift;
    const float yshift = Const::PathPlanner::PFLorinho::y_shift;
    const float factor = Const::PathPlanner::PFLorinho::factor;

    return (v.getUnitary() * k) / pow((d - xshift), factor) + yshift;
}

Vector PFLorinho::getVector(const Position &v1, const Position &v2) {
    return Vector(v2.x() - v1.x(), v2.y() - v1.y());
}

void PFLorinho::addRepulsivePosition(const Position &pos, float k) {
    std::pair<Position, float> repulsive = std::make_pair(pos, k);
    _repulsive.append(repulsive);
}

void PFLorinho::addCollisionPosition(const Position &pos, bool isBall) {
    _collisionPositions.push_back((std::make_pair(pos, isBall)));
}

Angle PFLorinho::getDirection() const {
    return Angle(true, _resultantAngle);
}

bool PFLorinho::isOnCollisionRoute(float resultantAngle) {
    // Check if colliding
    for (int i = 0; i < _collisionPositions.size(); i++) {
        const Position posObst = _collisionPositions.at(i).first;
        const bool posObstBall = _collisionPositions.at(i).second;
        const float distObst = WR::Utils::distance(originPos(), posObst);
        const float angleObst = WR::Utils::getAngle(originPos(), posObst);
        const float collisionRadius = (posObstBall ? Const::PathPlanner::PFLorinho::ball_collision_radius
                                                   : Const::PathPlanner::PFLorinho::robot_collision_radius);
        const float fix = atan2(collisionRadius, distObst);

        // Check collision distance
        if (distObst > Const::PathPlanner::PFLorinho::collision_check_radius)
            continue;

        // Check if goal is nearest than obstacle
        float distGoal = WR::Utils::distance(originPos(), goalPos());
        if (distGoal <= 0.95 * distObst) // applied factor to avoid small distance collisions
            continue;

        // Check if its a collision route
        float dif = WR::Utils::angleDiff(angleObst, resultantAngle);
        if (fabs(dif) <= 2 * fabs(fix))
            return true;
    }

    return false;
}

float PFLorinho::handleCollision(float resultantAngle) {

    // Put result. angle in known position
    WR::Utils::angleLimitZeroTwoPi(&resultantAngle);

    // Generate FreeAngles obstacle list
    QList<Lorinho::Obstacle> obstacles;
    for (int i = 0; i < _collisionPositions.size(); i++) {
        const Position posObst = _collisionPositions.at(i).first;
        const bool isObstBall = _collisionPositions.at(i).second;
        const float collisionRadius = (isObstBall ? Const::PathPlanner::PFLorinho::ball_collision_radius
                                                  : Const::PathPlanner::PFLorinho::robot_collision_radius);

        // Create obstacle
        Lorinho::Obstacle obst;
        obst.position() = posObst;
        obst.radius() = collisionRadius;

        // Add to list
        obstacles.push_back(obst);
    }

    // Run Free Angles
    const QList<Lorinho::FreeAngles::Interval> fa =
        Lorinho::FreeAngles::getFreeAngles(originPos(), 0, GEARSystem::Angle::twoPi, obstacles, true);

    // Get current resultant angle interval
    Lorinho::FreeAngles::Interval resultantInterval;
    bool resultantIntervalFound = false;
    for (int i = 0; i < fa.size(); i++) {
        Lorinho::FreeAngles::Interval interval = fa.at(i);

        float tmpResultAng = resultantAngle;

        // Exception case
        if (interval.angInitial() > interval.angFinal()) {
            if (tmpResultAng > interval.angInitial())
                tmpResultAng -= GEARSystem::Angle::twoPi;
            interval = Lorinho::FreeAngles::Interval(interval.angInitial() - GEARSystem::Angle::twoPi,
                                                     interval.angFinal(), interval.obstructed());
        }

        // Check
        if (interval.angInitial() <= tmpResultAng && tmpResultAng <= interval.angFinal()) {
            resultantInterval = interval;
            resultantIntervalFound = true;
            break;
        }
    }

    // Check if found an interval
    if (!resultantIntervalFound) {
        qCCritical(PF_CATEGORY)
            << "PF::handleCollision() not finding interval of resultant angle! (should never reach here";
        return resultantAngle;
    }

    // Check if interval is obstructed
    if (resultantInterval.obstructed()) {
        // Calculate to go right or left
        float difI = fabs(WR::Utils::angleDiff(resultantAngle, resultantInterval.angInitial()));
        float difF = fabs(WR::Utils::angleDiff(resultantAngle, resultantInterval.angFinal()));
        float result = difI <= difF ? resultantInterval.angInitial() : resultantInterval.angFinal();

        return result;
    }

    return resultantAngle;
}
