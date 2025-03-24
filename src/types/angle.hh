#include <cmath>

#include <iostream>

#define M_2_PI_EXCLUSIVE (2 * M_PI) - 1e-6
#define ANGLE_EPSILON 1e-6

enum class Quadrant { I = 1, II = 2, III = 3, IV = 4 };
enum class Clockwise { CW = 1, CCW = -1, EQUAL = 0 };

class Angle {
public:
    Angle(float radians = 0.0f) : value_(normalize(radians)) {}

    float radians() const {
        return value_;
    }

    void setRadians(float radians) {
        value_ = normalize(radians);
    }

    float degrees() const {
        return value_ * 180.0f / M_PI;
    }

    void setDegrees(float degrees) {
        value_ = normalize(degrees * M_PI / 180.0f);
    }

    Angle operator+(const Angle &other) const {
        return Angle(value_ + other.value_);
    }

    Angle operator-(const Angle &other) const {
        return Angle(value_ - other.value_);
    }

    Angle operator*(float scalar) const {
        return Angle(value_ * scalar);
    }

    Angle operator/(float scalar) const {
        return Angle(value_ / scalar);
    }

    Angle &operator+=(const Angle &other) {
        value_ = normalize(value_ + other.value_);
        return *this;
    }

    Angle &operator-=(const Angle &other) {
        value_ = normalize(value_ - other.value_);
        return *this;
    }

    Angle &operator*=(float scalar) {
        value_ = normalize(value_ * scalar);
        return *this;
    }

    Angle &operator/=(float scalar) {
        value_ = normalize(value_ / scalar);
        return *this;
    }

    bool operator==(const Angle &other) const {
        return normalize(std::fabs(normalize(value_) - normalize(other.value_))) < ANGLE_EPSILON;
    }

    bool operator!=(const Angle &other) const {
        return !(*this == other);
    }

    bool operator<(const Angle &other) const {
        return value_ < other.value_;
    }

    bool operator>(const Angle &other) const {
        return value_ > other.value_;
    }

    bool operator<=(const Angle &other) const {
        return value_ <= other.value_;
    }

    bool operator>=(const Angle &other) const {
        return value_ >= other.value_;
    }

    static bool isBetween(const Angle &angle, const Angle &start, const Angle &end) {
        float a = angle.radians();
        float s = start.radians();
        float e = end.radians();

        if (s < e) {
            return (a >= s - ANGLE_EPSILON) && (a < e - ANGLE_EPSILON);
        } else {
            return (a >= s - ANGLE_EPSILON) || (a < e - ANGLE_EPSILON);
        }
    }

    // Returns:
    //  1 if other is clockwise to this angle (right side)
    // -1 if other is counter-clockwise to this angle (left side)
    //  0 if angles are equal (within epsilon)
    static Clockwise relativeDirection(const Angle& pivot, const Angle& a2, float epsilon = 1e-5f) {
        float diff = normalize(a2.radians() - pivot.radians());
        
        if (diff < epsilon && diff > -epsilon) {
            return Clockwise::EQUAL; 
        }
        
        if (diff > 0 && diff < M_PI) {
            return Clockwise::CCW; 
        } else if (diff > M_PI) {
            return Clockwise::CCW; 
        } else {
            return Clockwise::CW; 
        }
    }

    static int size(const Angle &start, const Angle &end) {
        float s = start.radians();
        float e = end.radians();

        if (s < e) {
            return (e - s) / M_PI;
        } else {
            return (2.0f * M_PI - s + e) / M_PI;
        }
    }

    Quadrant quadrant() const {
        if (value_ < M_PI / 2.0f)
            return Quadrant::I;
        if (value_ < M_PI)
            return Quadrant::II;
        if (value_ < 3.0f * M_PI / 2.0f)
            return Quadrant::III;
        return Quadrant::IV;
    }

private:
    static float normalize(float radians) {
        radians = fmodf(radians, M_PI * 2.0f);
        if (radians < 0)
            radians += 2.0f * M_PI;
        return radians;
    }

    float value_;
};