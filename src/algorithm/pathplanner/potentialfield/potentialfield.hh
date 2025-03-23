#ifndef POTENTIALFIELD_HH
#define POTENTIALFIELD_HH
#include "algorithm/pathplanner/pathplanner.hh"

class PotentialField : public PathPlanner {
public:
    PotentialField(float kAtt, float kRep, float minRadiusForRepulsiveForce, float conicQuadraticThreshold,
                   float epsilon);

    QVector<Vec2> findPath(const Vec2 &start, const Vec2 &end, const QVector<Vec2> &obstacles);
    float findGreedyPath(const Vec2 &start, const Vec2 &end, const QVector<Vec2> &obstacles);

private:
    void addForce(const Vec2 &force);

    Vec2 resultantForce_;

    Vec2 origin_;
    Vec2 goal_;

    void addRepulsiveForce(const Vec2 &obstacle);
    void addAttractiveForce();

    Vec2 getForce() const;

    void reset();

    const float kAtt_;
    const float kRep_;
    const float minRadiusForRepulsiveForce_;
    const float conicQuadraticThreshold_;
    const float epsilon_;
    const float maxIts_ = 100;
    const float stepSize_ = 0.1;
};

#endif