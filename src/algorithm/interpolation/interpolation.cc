#include "interpolation.hh"

QVector<Vec2> Interpolation::smoothMovingAverage(const QVector<Vec2> &path, int window) {
    QVector<Vec2> out(path.size());
    int n = path.size();
    for (int i = 0; i < n; ++i) {
        Vec2 sum(0, 0);
        int count = 0;
        for (int j = std::max(0, i - window); j <= std::min(n - 1, i + window); ++j) {
            sum += path[j];
            count++;
        }
        out[i] = sum * (1.0f / count);
    }
    return out;
}

void Interpolation::smoothPathIterative(QVector<Vec2> &path, const QVector<Vec2> &orig, int iterations, float alpha,
                                        float beta) {
    int n = path.size();
    for (int it = 0; it < iterations; ++it) {
        for (int i = 1; i < n - 1; ++i) {
            Vec2 smoothTerm = path[i - 1] + path[i + 1] - 2.0f * path[i];
            Vec2 sourceTerm = orig[i] - path[i];

            path[i] += alpha * smoothTerm + beta * sourceTerm;
        }
    }
}