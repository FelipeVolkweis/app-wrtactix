#include "starpotential.hh"

StarPotential::StarPotential(float kAtt, float kRep, float minRadiusForRepulsiveForce, float astarToPFThreshold,
                             float epsilon)
    : potentialField_(kAtt, kRep, minRadiusForRepulsiveForce, 30, epsilon), astarToPFThreshold_(astarToPFThreshold) {}

QVector<Vec2> StarPotential::findPath(const Vec2 &start, const Vec2 &end, const QVector<Obstacle> &obstacles) {
    if ((start - end).norm() < astarToPFThreshold_) {
        return potentialField_.findPath(start, end, obstacles);
    } else {
        return aStar_.findPath(start, end, obstacles);
    }
}