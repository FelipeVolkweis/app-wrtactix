#ifndef NAVIGATION_HH
#define NAVIGATION_HH

#include <QVector>

#include "types/obstacle.hh"

class Navigation {
public:
    virtual void avoidTeammates() = 0;
    virtual void avoidOpponents() = 0;
    virtual void avoidBall() = 0;
    virtual void avoidOurGoal() = 0;
    virtual void avoidTheirGoal() = 0;
    virtual QVector<Vec2> findPath(const Vec2 &start, const Vec2 &goal, const QVector<Obstacle> &obstacles) = 0;
};

#endif