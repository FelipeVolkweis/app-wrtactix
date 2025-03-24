#ifndef RADIALSWEEP_HH
#define RADIALSWEEP_HH

#include <QVector>

#include "types/angle.hh"
#include "types/vec2.hh"

struct AngleInterval {
    AngleInterval(float start, float end) : start(start), end(end) {}

    AngleInterval(Angle start, Angle end) : start(start), end(end) {}

    AngleInterval() : start(0), end(0) {}

    Angle start;
    Angle end;
};

struct AngleEvent {
    Angle angle;
    bool start;
};

class RadialSweep {
public:
    RadialSweep(const Vec2 &observer, const QVector<Vec2> &obstacles, AngleInterval interval, float radius);
    QVector<AngleInterval> getFreeAngles() const;
    QVector<AngleInterval> getObstructedAngles() const;

    static QVector<AngleInterval> mergeObstructedIntervals(const QVector<AngleEvent> &events);
    static QVector<AngleEvent> createEvents(const QVector<Vec2> &obstacles, float obstacleRadius, const Vec2 &observer,
                                            float sweepRadius);
    static QVector<AngleInterval> getComplementIntervals(const QVector<AngleInterval> &intervals);

private:
    QVector<AngleInterval> freeAngles_;
    QVector<AngleInterval> obstructedAngles_;
    QVector<Vec2> obstacles_;
    Vec2 observer_;
    float radius_;
    AngleInterval interval_;

    void sweep();
};

#endif