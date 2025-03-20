#ifndef TWOD_HH
#define TWOD_HH

#include "types/vec2.hh"

#include <GEARSystem/Types/types.hh>

class TwoD {
public:
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
};

#endif