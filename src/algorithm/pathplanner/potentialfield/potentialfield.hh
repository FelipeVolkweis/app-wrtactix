#ifndef POTENTIALFIELD_HH
#define POTENTIALFIELD_HH

#include <eigen3/Eigen/Dense>

using Vec2 = Eigen::Vector2f;

class PotentialField {
public:
    PotentialField(float kAtt, float kRep, float minRadiusForRepulsiveForce, float conicQuadraticThreshold);

    void addRepulsiveForce(const Vec2 &obstacle);
    void addAttractiveForce();

    void setGoFromOriginToGoal(const Vec2 &origin, const Vec2 &goal);

    Vec2 getForce() const;

    void reset();

private:
    void addForce(const Vec2 &force);

    Vec2 resultantForce_;

    Vec2 origin_;
    Vec2 goal_;

    const float kAtt_;
    const float kRep_;
    const float minRadiusForRepulsiveForce_;
    const float conicQuadraticThreshold_;
};

#endif