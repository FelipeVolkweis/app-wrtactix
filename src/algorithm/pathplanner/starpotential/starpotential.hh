#ifndef STARPOTENTIAL_HH
#define STARPOTENTIAL_HH

#include "algorithm/pathplanner/astar/astar.hh"
#include "algorithm/pathplanner/pathplanner.hh"
#include "algorithm/pathplanner/potentialfield/potentialfield.hh"

class StarPotential : public PathPlanner {
public:
    StarPotential(float kAtt, float kRep, float minRadiusForRepulsiveForce, float astarToPFThreshold, float epsilon);
    QVector<Vec2> findPath(const Vec2 &start, const Vec2 &end, const QVector<Obstacle> &obstacles) override;

private:
    PotentialField potentialField_;
    AStar aStar_;

    const float astarToPFThreshold_;
};

#endif // STARPOTENTIAL_HH