#ifndef VEC2_HH
#define VEC2_HH

#include <QString>

#include <eigen3/Eigen/Dense>

using Vec2 = Eigen::Vector2f;

static const Vec2 NULL_VEC;

static bool isNullVec(const Vec2 &vec) {
    return vec == NULL_VEC;
}

static QString vec2ToString(const Vec2 &vec) {
    return QString("(%1, %2)").arg(vec[0]).arg(vec[1]);
}

#endif