#ifndef POINTTOPOINT_HH
#define POINTTOPOINT_HH

#include "algorithm/pathplanner/pathplanner.hh"

class PointToPoint : public PathPlanner {
public:
    PointToPoint();

    QVector<Vec2> findPath(const Vec2 &start, const Vec2 &goal, const QVector<Obstacle> &obstacles);
};

#endif