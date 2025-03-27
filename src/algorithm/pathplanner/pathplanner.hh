#ifndef PATHPLANNER_HH
#define PATHPLANNER_HH

#include "types/obstacle.hh"

#include "QVector"

class PathPlanner {
public:
    virtual QVector<Vec2> findPath(const Vec2 &start, const Vec2 &goal, const QVector<Obstacle> &obstacles) = 0;
};

#endif // PATHPLANNER_HH