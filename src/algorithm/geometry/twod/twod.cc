#include "twod.hh"

Vec2 TwoD::threePoints(const Vec2 &near, const Vec2 &far, float distance, float beta) {
    Angle alpha(true, atan2(far.y() - near.y(), far.x() - near.x()));
    Angle gama(true, alpha.value() + beta);
    Vec2 p(near.x() + distance * cos(gama.value()), near.y() + distance * sin(gama.value()));
    return p;
}

bool TwoD::isPointOnSegment(float x, float y, float x1, float y1, float x2, float y2) {
    float cross = (x - x1) * (y2 - y1) - (y - y1) * (x2 - x1);
    if (abs(cross) > 1e-6)
        return false;
    float dot = (x - x1) * (x2 - x1) + (y - y1) * (y2 - y1);
    if (dot < 0)
        return false;
    float squaredLength = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
    if (dot > squaredLength)
        return false;
    return true;
}

bool TwoD::isPointOnSegment(const Vec2 &point, const Vec2 &p1, const Vec2 &p2) {
    return isPointOnSegment(point.x(), point.y(), p1.x(), p1.y(), p2.x(), p2.y());
}

Vec2 TwoD::infiniteLineIntersection(const Line &line1, const Line &line2) {
    float x1 = line1.p1.x();
    float y1 = line1.p1.y();
    float x2 = line1.p2.x();
    float y2 = line1.p2.y();
    float x3 = line2.p1.x();
    float y3 = line2.p1.y();
    float x4 = line2.p2.x();
    float y4 = line2.p2.y();

    float denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (fabsf(denom) < 1e-6) {
        return NULL_VEC; // Lines are parallel or coincident
    }

    float x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) /
              ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
    float y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) /
              ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

    return Vec2(x, y);
}

Vec2 TwoD::boundedLineIntersection(const Line &line1, const Line &line2) {
    Vec2 intersection = infiniteLineIntersection(line1, line2);
    if (intersection == NULL_VEC) {
        return NULL_VEC;
    }

    if (isPointOnSegment(intersection, line1.p1, line1.p2) && isPointOnSegment(intersection, line2.p1, line2.p2)) {
        return intersection;
    }

    return NULL_VEC;
}

Vec2 TwoD::boundedLineIntersection(const Line &line, const Vec2 &center, float radius) {
    Vec2 p1 = line.p1;
    Vec2 p2 = line.p2;

    float dx = p2.x() - p1.x();
    float dy = p2.y() - p1.y();

    float A = dx * dx + dy * dy;
    float B = 2 * (dx * (p1.x() - center.x()) + dy * (p1.y() - center.y()));
    float C =
        (p1.x() - center.x()) * (p1.x() - center.x()) + (p1.y() - center.y()) * (p1.y() - center.y()) - radius * radius;

    float det = B * B - 4 * A * C;
    if ((A <= 1e-6) || (det < 0)) {
        return NULL_VEC; // No intersection
    }

    float t1 = (-B - sqrt(det)) / (2 * A);
    float t2 = (-B + sqrt(det)) / (2 * A);

    if ((t1 >= 0 && t1 <= 1) || (t2 >= 0 && t2 <= 1)) {
        if (t1 >= 0 && t1 <= 1) {
            return Vec2(p1.x() + t1 * dx, p1.y() + t1 * dy);
        } else {
            return Vec2(p1.x() + t2 * dx, p1.y() + t2 * dy);
        }
    }

    return NULL_VEC; // Intersection is outside the segment
}

QPair<Vec2, float> TwoD::findCircleFromThreePoints(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3) {
    Vec2 mid12((p1.x() + p2.x()) / 2, (p1.y() + p2.y()) / 2);
    Vec2 mid23((p2.x() + p3.x()) / 2, (p2.y() + p3.y()) / 2);

    float slope12 = (p2.y() - p1.y()) / (p2.x() - p1.x());
    float slope23 = (p3.y() - p2.y()) / (p3.x() - p2.x());

    float perpSlope12 = -1 / slope12;
    float perpSlope23 = -1 / slope23;

    float b1 = mid12.y() - perpSlope12 * mid12.x();
    float b2 = mid23.y() - perpSlope23 * mid23.x();

    float centerX = (b2 - b1) / (perpSlope12 - perpSlope23);
    float centerY = perpSlope12 * centerX + b1;
    Vec2 center(centerX, centerY);

    float radius = distance(center, p1);

    return QPair<Vec2, float>(center, radius);
}

QVector<Vec2> TwoD::findLineCircleIntersections(const Vec2 &center, float radius, float m, float b) {
    QVector<Vec2> intersections;

    // Coefficients for the quadratic equation
    float A = 1 + qPow(m, 2);
    float B = -2 * center.x() + 2 * m * (b - center.y());
    float C = qPow(center.x(), 2) + qPow(b - center.y(), 2) - qPow(radius, 2);

    // Discriminant
    float discriminant = qPow(B, 2) - 4 * A * C;

    if (discriminant > 0) {
        // Two intersection points
        float x1 = (-B + qSqrt(discriminant)) / (2 * A);
        float x2 = (-B - qSqrt(discriminant)) / (2 * A);

        // Corresponding y values
        float y1 = m * x1 + b;
        float y2 = m * x2 + b;

        intersections.append(Vec2(x1, y1));
        intersections.append(Vec2(x2, y2));
    } else if (discriminant == 0) {
        // One intersection point (tangent)
        float x = -B / (2 * A);
        float y = m * x + b;

        intersections.append(Vec2(x, y));
    }

    return intersections;
}