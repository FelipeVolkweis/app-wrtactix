#ifndef VEC2_HH
#define VEC2_HH

#include <eigen3/Eigen/Dense>

using Vec2 = Eigen::Vector2f;

static const Vec2 NULL_VEC;

static bool isNullVec(const Vec2 &vec) {
    return vec == NULL_VEC;
}

#endif