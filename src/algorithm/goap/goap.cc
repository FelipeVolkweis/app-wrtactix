#include "goap.hh"

Goap::Goap(QVector<Behavior *> availableBehaviors) {}

bool Goap::buildPlan(QSharedPointer<GoapNode> step, const Dictionary<bool> &blackboard) {
    bool hasFollowUp = false;
    auto state = step->desiredState();

    for (const auto &s : state.asKeyValueRange()) {
        if (state[s.first] == blackboard[s.first]) {
            state.remove(s.first);
        }
    }

    if (state.isEmpty()) {
        return true;
    }

    for (auto b : availableBehaviors_) {
        if (!b->isValid()) {
            continue;
        }

        bool shouldAddToPlan = false;
        auto effects = b->effects();
        auto desiredState = state;

        for (const auto &s : desiredState.asKeyValueRange()) {
            if (desiredState[s.first] == effects[s.first]) {
                desiredState.remove(s.first);
                shouldAddToPlan = true;
            }
        }

        if (shouldAddToPlan) {
            for (const auto &precon : b->preconditions().asKeyValueRange()) {
                const auto &s = precon.first;
                desiredState[s] = b->preconditions()[s];
            }

            QSharedPointer<GoapNode> node(new GoapNode(b, desiredState));

            if (desiredState.isEmpty() || buildPlan(node, blackboard)) {
                step->addChild(node);
                hasFollowUp = true;
            }
        }
    }

    return hasFollowUp;
}

QVector<Plan> Goap::getAllPlansFromTree(GoapNode *root, const Dictionary<bool> &blackboard) {
    QVector<Plan> plans;

    if (root == nullptr) {
        return QVector<Plan>();
    }

    if (root->isLeaf()) {
        plans.push_back({ QVector<Behavior *> {root->behavior()},  root->behavior()->cost() });
        return plans;
    }

    for (auto &c : root->getChildren()) {
        for (auto &newPlan : getAllPlansFromTree(c.data(), blackboard)) {
            if (root->hasBehavior()) {
                newPlan.behaviors.append(root->behavior());
                
            }
        }
    }
}
