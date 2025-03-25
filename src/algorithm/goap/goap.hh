#ifndef GOAP_HH
#define GOAP_HH

#include <QSharedPointer>
#include <QVector>

#include "types/dictionary.hh"

#include "behavior.hh"

class GoapNode {
public:
    GoapNode() : behavior_(nullptr) {}

    GoapNode(Behavior *behavior, const Dictionary<bool> &desiredState)
        : behavior_(behavior), desiredState_(desiredState) {};

    bool hasBehavior() const {
        return behavior_ != nullptr;
    }

    Behavior *behavior() {
        return behavior_;
    }

    Dictionary<bool> &desiredState() {
        return desiredState_;
    }

    void addChild(QSharedPointer<GoapNode> c) {
        children_.append(c);
    }

    QVector<QSharedPointer<GoapNode>> getChildren() {
        return children_;
    }

    bool isLeaf() const {
        return children_.length() == 0;
    }

private:
    Behavior *behavior_;
    QVector<QSharedPointer<GoapNode>> children_;
    Dictionary<bool> desiredState_;
};

struct Plan {
    QVector<Behavior *> behaviors;
    float cost;
};

class Goap {
public:
    Goap(QVector<Behavior *> availableBehaviors);

private:
    bool buildPlan(QSharedPointer<GoapNode> step, const Dictionary<bool> &blackboard);
    QVector<Plan> getAllPlansFromTree(GoapNode *root, const Dictionary<bool> &blackboard);

    QVector<Behavior *> availableBehaviors_;
};

#endif