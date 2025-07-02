#ifndef PF_HH
#define PF_HH

#include "algorithm/pathplanner/pathplanner.hh"

#include "vector.hh"

#include <GEARSystem/Types/types.hh>

class PFLorinho : public PathPlanner {
public:
    PFLorinho();

    QVector<Vec2> findPath(const Vec2 &start, const Vec2 &end, const QVector<Obstacle> &obstacles);

private:
    QString name();

    // Setters
    void reset();
    void addBall(const Position &pos);
    void addOwnRobot(const Position &pos);
    void addEnemyRobot(const Position &pos);
    void addGoalArea(const Position &pos);

    // Run
    void run();

    // Results
    Angle getDirection() const;

    const Position &originPos() const {
        return _originPos;
    }

    const Position &goalPos() const {
        return _goalPos;
    }

    // Collision checking
    QList<std::pair<Position, bool>> _collisionPositions;
    float handleCollision(float resultantAngle);
    bool isOnCollisionRoute(float resultantAngle);

    // Internal addRobot
    void addRobot(const Position &pos);

    // Forces
    void addRepulsiveForce(const Vector &v, float k);
    void addAttractiveForce(const Vector &v);
    void addForce(const Vector &v);

    // Distance function
    Vector applyDistanceFunction(Vector v, float k);

    // Auxiliary functions
    static Vector getVector(const Position &v1, const Position &v2);
    void addRepulsivePosition(const Position &pos, float k);
    void addCollisionPosition(const Position &pos, bool isBall = false);

    // Repulsive obstacles
    QList<std::pair<Position, float>> _repulsive;

    // Results
    Vector _resultantForce;
    float _resultantAngle;

    Position _originPos;
    Position _goalPos;
};

#endif // PF_HH
