#include "pointtopoint.hh"

PointToPoint::PointToPoint() {}

QVector<Vec2> PointToPoint::findPath(const Vec2 &start, const Vec2 &goal, const QVector<Obstacle> &obstacles) {
    QVector<Vec2> path;
    path.append(start);
    path.append(goal);
    return path;
}