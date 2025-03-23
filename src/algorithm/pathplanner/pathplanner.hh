#ifndef PATHPLANNER_HH
#define PATHPLANNER_HH

#include "types/vec2.hh"

#include "QVector"

class PathPlanner {
public:
    virtual QVector<Vec2> findPath(const Vec2 &start, const Vec2 &goal, const QVector<Vec2> &obstacles) = 0;
};

#endif // PATHPLANNER_HH