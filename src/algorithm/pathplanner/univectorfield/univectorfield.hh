#ifndef UNIVECTORFIELD_HH
#define UNIVECTORFIELD_HH

#include "algorithm/pathplanner/pathplanner.hh"

class UnivectorField : PathPlanner {
public:
    UnivectorField();
    UnivectorField(float de, float kr, float ko, float dmin, float delta, float maxits, float step);
    QVector<Vec2> findPath(const Vec2 &start, const Vec2 &goal, const QVector<Obstacle> &obstacles);

private:
    float wrapToPi(float angle);
    float atan2Pi(float y, float x);
    float gaussian(float r, float delta);
    float phiH(float rho, float theta, float de, float kr, bool cw);
    float phiR(const Vec2 &delta);
    float phiTuf(float theta, const Vec2 &deltaGoal, float de);
    float phiAuf(const Vec2 &obstaclePos, const Vec2 &robotPos, const Vec2 &vObs, const Vec2 &vRob, float ko);
    float phiComposed(float phiTuf, float phiAuf, float R, const QVector<Obstacle> &obstacles, float delta, float dMin);
    Vec2 getDirection(const Vec2 &originPos, const Vec2 &goalPos, const QVector<Obstacle> &obstacles, const Vec2 &vObs,
                      const Vec2 &vRob);
    Vec2 getDirection(const Vec2 &originPos, const Vec2 &goalPos, const QVector<Obstacle> &obstacles);
    Vec2 closestObstacle(const Vec2 &robotPos, const QVector<Obstacle> &obstacles);
    QVector<Vec2> generatePath(const Vec2 &originPos, const Vec2 &goalPos, const QVector<Obstacle> &obstacles,
                               const Vec2 &vObs, const Vec2 &vRob);

    Vec2 getObstacleSurface(const Obstacle &obstacle, const Vec2 &origin);

    float de_ = 10 / 100.0f;   // predefined radius for spiral
    float kr_ = 0.5 / 100.0f;  // adjustable parameter for spiral
    float ko_ = 1;             // parameter for obstacle avoidance
    float dMin_ = 5 / 100.0f;  // minimum distance for obstacle influence
    float delta_ = 5 / 100.0f; // parameter for Gaussian function
    float maxIts_ = 1000;
    float step_ = 0.05;
};

#endif