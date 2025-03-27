#include "algorithm/geometry/twod/twod.hh"

#include "rrtconnect.hh"

#define MM2M 0.001

RRTConnect::RRTConnect() {}

QVector<Vec2> RRTConnect::findPath(const Vec2 &start, const Vec2 &end, const QVector<Obstacle> &obstacles) {
    QVector<Vec2> path;
    KDTree *treeA = new KDTree();
    KDTree *treeB = new KDTree();

    QHash<KDNode *, KDNode *> *parentsA = new QHash<KDNode *, KDNode *>();
    QHash<KDNode *, KDNode *> *parentsB = new QHash<KDNode *, KDNode *>();

    KDNode *startNode = treeA->insert(start);
    KDNode *endNode = treeB->insert(end);

    KDNode *closestNode = startNode;
    bool found = false;

    Vec2 currentGoal = end;
    obstacles_ = obstacles;

    for (int k = 0; k < maxIts_; k++) {
        Vec2 qRand = randomSampleWithGoalBias(currentGoal);

        auto resultA = extend(*treeA, *parentsA, qRand);
        if (resultA.status != RRTConnectStatus::TRAPPED) {
            Vec2 q = resultA.qNew->point();

            if ((q - closestNode->point()).norm() < (end - closestNode->point()).norm()) {
                closestNode = resultA.qNew;
            }

            auto resultB = connect(*treeB, *parentsB, q);
            if (resultB.status == RRTConnectStatus::REACHED) {
                // Find meeting node in treeB (q is the meeting point)
                KDNode *meetingNodeB = treeB->nearestNeighbor(q);
                path = makePath(*parentsA, *parentsB, resultA.qNew, meetingNodeB);
                found = true;
                break;
            }
        }

        std::swap(treeA, treeB);
        std::swap(parentsA, parentsB);
        if (currentGoal == end) {
            currentGoal = start;
        } else {
            currentGoal = end;
        }
    }

    if (!found) {
        path = makePath(*parentsA, *parentsB, closestNode, nullptr);
    }

    delete treeA;
    delete treeB;
    delete parentsA;
    delete parentsB;

    obstacles_.clear();

    return path;
}

QVector<Vec2> RRTConnect::makePath(ParentMap &pA, ParentMap &pB, KDNode *meetingNodeA, KDNode *meetingNodeB) {
    QVector<Vec2> path;

    // Path from start to meeting point (treeA)
    KDNode *node = meetingNodeA;
    while (node != nullptr) {
        path.push_front(node->point()); // Reverse to start from the start
        node = pA.value(node, nullptr);
    }

    // Path from meeting point to end (treeB, excluding meeting point)
    if (meetingNodeB != nullptr) {
        node = pB.value(meetingNodeB, nullptr);
        while (node != nullptr) {
            path.append(node->point());
            node = pB.value(node, nullptr);
        }
    }

    return path;
}

ExtendResult RRTConnect::connect(KDTree &T, ParentMap &p, const Vec2 &q) {
    RRTConnectStatus status = RRTConnectStatus::TRAPPED;
    ExtendResult result;
    int its = 0;
    do {
        result = extend(T, p, q);
        status = result.status;
        its++;

        if (its > maxIts_) {
            status = RRTConnectStatus::TRAPPED;
            break;
        }
    } while (status == RRTConnectStatus::ADVANCED);

    return result;
}

ExtendResult RRTConnect::extend(KDTree &T, ParentMap &p, const Vec2 &q) {
    auto qNear = T.nearestNeighbor(q);
    float dist = (q - qNear->point()).norm();
    Vec2 qNewCandidate = qNear->point() + fminf(dist, stepSize()) * (q - qNear->point()).normalized();

    // Collision check
    bool collision = false;
    for (const auto &o : obstacles_) {
        TwoD::Line line(qNear->point(), qNewCandidate);
        if (TwoD::boundedLineIntersection(line, o.center, o.radius) != NULL_VEC) {
            collision = true;
            break;
        }
    }

    if (!collision) {
        auto qNew = T.insert(qNewCandidate);
        p[qNew] = qNear;

        if ((qNew->point() - q).norm() < goalRadius_) { // Use epsilon check
            return {RRTConnectStatus::REACHED, qNew};
        } else {
            return {RRTConnectStatus::ADVANCED, qNew};
        }
    }
    return {RRTConnectStatus::TRAPPED, nullptr};
}

Vec2 RRTConnect::randomSample() {
    float x = QRandomGenerator::global()->bounded(0, 9000) * MM2M - 4.5;
    float y = QRandomGenerator::global()->bounded(0, 6000) * MM2M - 3;
    return Vec2(x, y);
}

Vec2 RRTConnect::randomSampleWithGoalBias(const Vec2 &goal) {
    if (QRandomGenerator::global()->bounded(0, 1) < goalBiasing_) {
        return goal;
    } else {
        return randomSample();
    }
}

float RRTConnect::varyingStepSize(const Vec2 &q, const Vec2 &qNear) {
    float dist = (q - qNear).norm();

    if (QRandomGenerator::global()->bounded(0, 1) < 0.9) {
        return stepSize_;
    }
    return dist;
}

float RRTConnect::stepSize() {
    return stepSize_;
}