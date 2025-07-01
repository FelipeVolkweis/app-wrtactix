#ifndef INTERPOLATION_HH
#define INTERPOLATION_HH

#include <QVector>

#include "types/vec2.hh"

class Interpolation {
public:
    static void smoothPathIterative(QVector<Vec2> &path, const QVector<Vec2> &orig, int iterations = 50,
                                    float alpha = 0.1f, float beta = 0.3f);
    static QVector<Vec2> smoothMovingAverage(const QVector<Vec2> &path, int window);
};

#endif