#include "radialsweep.hh"
#include <algorithm>

RadialSweep::RadialSweep(const Vec2 &observer, const QVector<Vec2> &obstacles, AngleInterval interval, float radius)
    : observer_(observer), obstacles_(obstacles), interval_(interval), radius_(radius) {
    sweep();
}

void RadialSweep::sweep() {
    QVector<AngleEvent> events = createEvents(obstacles_, 0.09, observer_, radius_);
}

QVector<AngleEvent> RadialSweep::createEvents(const QVector<Vec2> &obstacles, float obstacleRadius,
                                              const Vec2 &observer, float sweepRadius) {
    QVector<AngleEvent> events;
    for (const Vec2 &obstacle : obstacles) {
        Vec2 diff = obstacle - observer;
        float distance = diff.norm();
        if (distance > sweepRadius) {
            continue;
        }
        Angle angle(atan2f(diff.y(), diff.x()));
        Angle alpha(asinf(obstacleRadius / distance));

        Angle a1 = angle - alpha;
        Angle a2 = angle + alpha;

        if (a1.quadrant() == Quadrant::IV && a2.quadrant() == Quadrant::I) {
            events.push_back({Angle(0), true});
            events.push_back({a1, true});
            events.push_back({a2, false});
            events.push_back({Angle(M_2_PI_EXCLUSIVE), false});
        } else {
            events.push_back({a1, true});
            events.push_back({a2, false});
        }
    }

    std::sort(events.begin(), events.end(), [](const AngleEvent &a, const AngleEvent &b) { return a.angle < b.angle; });

    return events;
}

QVector<AngleInterval> RadialSweep::mergeObstructedIntervals(const QVector<AngleEvent> &events) {
    QVector<AngleInterval> intervals;
    int count = 0;

    AngleInterval currentInterval;
    for (const auto &e : events) {
        if (e.start) {
            count++;
            if (count == 1) {
                currentInterval.start = e.angle;
            }
        } else {
            count--;
            if (count == 0) {
                currentInterval.end = e.angle;
                intervals.push_back(currentInterval);
            }
        }
    }
    return intervals;
}

QVector<AngleInterval> RadialSweep::getComplementIntervals(const QVector<AngleInterval> &intervals) {
    QVector<AngleInterval> complement;

    if (intervals.empty()) {
        complement.push_back({Angle(0), Angle(M_2_PI_EXCLUSIVE)});
        return complement;
    }

    if (intervals[0].start != Angle(0)) {
        complement.push_back({Angle(0), intervals[0].start});
    }

    for (int i = 0; i < intervals.size() - 1; i++) {
        complement.push_back({intervals[i].end, intervals[i + 1].start});
    }

    if (intervals[intervals.size() - 1].end != Angle(M_2_PI_EXCLUSIVE)) {
        complement.push_back({intervals[intervals.size() - 1].end, Angle(M_2_PI_EXCLUSIVE)});
    }

    return complement;
}