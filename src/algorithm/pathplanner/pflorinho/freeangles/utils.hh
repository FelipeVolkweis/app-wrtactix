#ifndef UTILS_HH_
#define UTILS_HH_

#include <GEARSystem/Types/types.hh>

namespace WR {
struct Utils {
    static void angleLimitZeroTwoPi(float *angle) {
        while (*angle < 0)
            *angle += GEARSystem::Angle::twoPi;
        while (*angle > GEARSystem::Angle::twoPi)
            *angle -= GEARSystem::Angle::twoPi;
    }

    static float distance(const Position &a, const Position &b) {
        return sqrt(pow(a.x() - b.x(), 2) + pow(a.y() - b.y(), 2));
    }

    static float getAngle(const Position &a, const Position &b) {
        return atan2(b.y() - a.y(), b.x() - a.x());
    }

    static float angleDiff(float A, float B) {
        /* Here we face several problems caused by the
        ** the non-linearity of angles... Say we have
        ** A = -pi and B = 2pi, both angles are properly
        ** represented, but B-A = 3pi!!!! (to be fair, it
        ** isn't wrong [3pi = pi], but isn't the result
        ** that we expect). We face a worst problem if the
        ** angle is not represented in an interval that
        ** contain 0... To fix it, we have to make sure
        ** that the angles are in the same interval..
        **/
        angleLimitZeroTwoPi(&A);
        angleLimitZeroTwoPi(&B);

        float diff = fabs(B - A);
        if (diff > GEARSystem::Angle::pi)
            diff = GEARSystem::Angle::twoPi - diff;
        return diff;
    }

    static float angleDiff(const Angle &A, const float B) {
        return angleDiff(A.value(), B);
    }

    static float angleDiff(const float A, const Angle &B) {
        return angleDiff(A, B.value());
    }

    static float angleDiff(const Angle &A, const Angle &B) {
        return angleDiff(A.value(), B.value());
    }
};
} // namespace WR

#endif