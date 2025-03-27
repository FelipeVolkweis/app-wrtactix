#ifndef OBSTACLE_HH
#define OBSTACLE_HH

#include "types/vec2.hh"

enum class ObstacleType { CIRCLE, RECTANGLE };

class Obstacle {
public:
    Obstacle(const Vec2 &center, float radius) : center(center), radius(radius), type(ObstacleType::CIRCLE) {}

    Obstacle(const Vec2 &bottomLeft, const Vec2 &topRight)
        : bottomLeft(bottomLeft), topRight(topRight), type(ObstacleType::RECTANGLE) {}

    bool isCircle() const {
        return type == ObstacleType::CIRCLE;
    }

    bool isRectangle() const {
        return type == ObstacleType::RECTANGLE;
    }

    Vec2 center;
    float radius;

    Vec2 bottomLeft;
    Vec2 topRight;

    ObstacleType type;
};

#endif // OBSTACLE_HH