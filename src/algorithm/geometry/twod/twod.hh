#ifndef TWOD_HH
#define TWOD_HH

#include "types/vec2.hh"

#include <GEARSystem/Types/types.hh>

class TwoD {
public:
    struct Line {
        Line(const Vec2 &p1, const Vec2 &p2) : p1(p1), p2(p2) {}

        Vec2 p1;
        Vec2 p2;
    };

    static Vec2 threePoints(const Vec2 &near, const Vec2 &far, float distance, float beta);

    static float distance(const GEARSystem::Position &p1, const GEARSystem::Position &p2) {
        return sqrtf(powf(p1.x() - p2.x(), 2) + powf(p1.y() - p2.y(), 2));
    }

    static float distance(const Vec2 &p1, const Vec2 &p2) {
        return sqrtf(powf(p1[0] - p2[0], 2) + powf(p1[1] - p2[1], 2));
    }

    static GEARSystem::Position vectorToPosition(const Vec2 &vec) {
        return GEARSystem::Position(true, vec[0], vec[1], 0);
    }

    static Vec2 positionToVector(const GEARSystem::Position &pos) {
        return Vec2(pos.x(), pos.y());
    }

    static Vec2 velocityToVector(const GEARSystem::Velocity &vel) {
        return Vec2(vel.x(), vel.y());
    }

    static bool isPointOnSegment(float x, float y, float x1, float y1, float x2, float y2);

    static bool isPointOnSegment(const Vec2 &point, const Vec2 &p1, const Vec2 &p2);

    static Vec2 infiniteLineIntersection(const Line &line1, const Line &line2);

    static Vec2 boundedLineIntersection(const Line &line, const Line &line2);

    static Vec2 boundedLineIntersection(const Line &line, const Vec2 &center, float radius);

    static QPair<Vec2, float> findCircleFromThreePoints(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3);

    static QVector<Vec2> findLineCircleIntersections(const Vec2 &center, float radius, float m, float b);

    static float angleBetweenVectors(const Vec2 &v1, const Vec2 &v2) {
        return atan2f(v2.y() - v1.y(), v2.x() - v1.x());
    }

    static float angleBetweenPositions(const Position &p1, const Position &p2) {
        return atan2f(p2.y() - p1.y(), p2.x() - p1.x());
    }

    static float angleDiff(float a1, float a2) {
        return angleMinusPiToPi(a1 - a2);
    }

    static float angleMinusPiToPi(float a) {
        while (a > M_PI) {
            a -= 2 * M_PI;
        }
        while (a < -M_PI) {
            a += 2 * M_PI;
        }

        return a;
    }

    static Vec2 rotateVec2(const Vec2 &v, float thetaRad) {
        float cosTheta = cosf(thetaRad);
        float sinTheta = sinf(thetaRad);
        return Vec2(
            v.x() * cosTheta - v.y() * sinTheta,
            v.x() * sinTheta + v.y() * cosTheta
        );
    }
};

#endif