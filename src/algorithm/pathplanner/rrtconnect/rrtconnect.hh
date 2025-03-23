#ifndef RRTCONNECT_HH
#define RRTCONNECT_HH

#include <QVector>

#include "algorithm/geometry/kdtree/kdtree.hh"
#include "algorithm/pathplanner/pathplanner.hh"

using ParentMap = QHash<KDNode *, KDNode *>;

enum class RRTConnectStatus { REACHED, ADVANCED, TRAPPED };

struct ExtendResult {
    RRTConnectStatus status;
    KDNode *qNew;
};

class RRTConnect : public PathPlanner {
public:
    RRTConnect();

    QVector<Vec2> findPath(const Vec2 &start, const Vec2 &end, const QVector<Vec2> &obstacles);

private:
    ExtendResult connect(KDTree &T, ParentMap &p, const Vec2 &q);
    ExtendResult extend(KDTree &T, ParentMap &p, const Vec2 &q);
    Vec2 randomSample();
    Vec2 randomSampleWithGoalBias(const Vec2 &goal);
    float varyingStepSize(const Vec2 &q, const Vec2 &qNear);
    float stepSize();
    QVector<Vec2> makePath(ParentMap &pA, ParentMap &pB, KDNode *meetingNodeA, KDNode *meetingNodeB);

    QVector<Vec2> obstacles_;

    const int maxIts_ = 100;
    const float stepSize_ = 0.1;
    const float goalRadius_ = 0.05;
    const float obstacleRadius_ = 0.09;
    const float goalBiasing_ = 0.00;
};

#endif