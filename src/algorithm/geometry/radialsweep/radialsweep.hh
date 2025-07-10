#ifndef RADIALSWEEP_HH
#define RADIALSWEEP_HH

#include <QVector>

#include "types/angle.hh"
#include "types/vec2.hh"

struct AngleInterval {
    AngleInterval(float start, float end) : start(start), end(end) {}

    AngleInterval(WRAngle start, WRAngle end) : start(start), end(end) {}

    AngleInterval() : start(0), end(0) {}

    WRAngle start;
    WRAngle end;

    bool operator==(const AngleInterval &other) const {
        return start == other.start && end == other.end;
    }

    bool operator!=(const AngleInterval &other) const {
        return !(*this == other);
    }

    float size() const {
        return WRAngle::size(start, end);
    }

    bool isInsideInterval(const WRAngle &angle) const {
        return WRAngle::isBetween(angle, start, end);
    }
};

struct AngleEvent {
    WRAngle angle;
    bool start;
};

class RadialSweep {
public:
    RadialSweep(const Vec2 &observer, const QVector<Vec2> &obstacles, float obstacleRadius, AngleInterval interval,
                float radius);
    RadialSweep(const Vec2 &observer, const QVector<Vec2> &obstacles, AngleInterval interval, float radius);

    QVector<AngleInterval> getFreeAngles() const {
        return freeAngles_;
    };

    QVector<AngleInterval> getObstructedAngles() const {
        return obstructedAngles_;
    };

    static QVector<AngleInterval> mergeObstructedIntervals(const QVector<AngleEvent> &events);
    static QVector<AngleEvent> createEvents(const QVector<Vec2> &obstacles, float obstacleRadius, const Vec2 &observer,
                                            float sweepRadius, AngleInterval interval);
    static QVector<AngleInterval> getComplementIntervals(const QVector<AngleInterval> &intervals,
                                                         AngleInterval interval);

    static AngleInterval getLargestAngleInterval(QVector<AngleInterval> intervals);
    static WRAngle getCenterOfInterval(const AngleInterval &interval);

private:
    QVector<AngleInterval> freeAngles_;
    QVector<AngleInterval> obstructedAngles_;
    QVector<Vec2> obstacles_;
    Vec2 observer_;
    float radius_;
    AngleInterval interval_;
    float obstacleRadius_;

    void sweep();
};

#endif