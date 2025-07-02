#ifndef OBSTACLE_HH
#define OBSTACLE_HH

#include "types/vec2.hh"

enum class ObstacleType { CIRCLE, RECTANGLE };

enum class CircularObstacleType { UNKNOWN, TEAMMATE, OPPONENT, BALL };

class Obstacle {
public:
    Obstacle(const Vec2 &center, float radius) : center(center), radius(radius), type(ObstacleType::CIRCLE) {}

    Obstacle(const Vec2 &center, float radius, CircularObstacleType whichKind)
        : center(center), radius(radius), type(ObstacleType::CIRCLE), circularType(whichKind) {}

    Obstacle(const Vec2 &bottomLeft, const Vec2 &topRight)
        : bottomLeft(bottomLeft), topRight(topRight), type(ObstacleType::RECTANGLE) {}

    bool isCircle() const {
        return type == ObstacleType::CIRCLE;
    }

    bool isRectangle() const {
        return type == ObstacleType::RECTANGLE;
    }

    bool isKnownCircularType() const {
        return circularType != CircularObstacleType::UNKNOWN;
    }

    bool isTeammate() const {
        return circularType == CircularObstacleType::TEAMMATE;
    }

    bool isOpponent() const {
        return circularType == CircularObstacleType::OPPONENT;
    }

    bool isBall() const {
        return circularType == CircularObstacleType::BALL;
    }

    Vec2 center;
    float radius;

    Vec2 bottomLeft;
    Vec2 topRight;

    ObstacleType type;
    CircularObstacleType circularType = CircularObstacleType::UNKNOWN;
};

#endif // OBSTACLE_HH