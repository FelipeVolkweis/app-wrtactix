#include "twod.hh"

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