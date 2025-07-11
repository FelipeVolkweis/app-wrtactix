#ifndef BEHAVIORTREE_HH
#define BEHAVIORTREE_HH

#include "base/basenodes.hh"

namespace WRBeT {
class BehaviorTree {
public:
    static inline Sequence *Sequence(const QString &name, std::initializer_list<Node *> children) {
        auto sequence = new WRBeT::Sequence(name);
        for (auto child : children) {
            sequence->addChild(child);
        }
        return sequence;
    }

    static inline Fallback *Fallback(const QString &name, std::initializer_list<Node *> children) {
        auto fallback = new WRBeT::Fallback(name);
        for (auto child : children) {
            fallback->addChild(child);
        }
        return fallback;
    }

    static inline Parallel *Parallel(const QString &name, std::initializer_list<Node *> children) {
        auto parallel = new WRBeT::Parallel(name);
        for (auto child : children) {
            parallel->addChild(child);
        }
        return parallel;
    }

    static inline Condition *Condition(const QString &name, std::function<bool()> conditionTrue) {
        return new WRBeT::Condition(name, conditionTrue);
    }

    static inline RunLambda *RunLambda(const QString &name, std::function<void()> lambda) {
        return new WRBeT::RunLambda(name, lambda);
    }

    static inline ForceFailure *ForceFailure(Node *child) {
        return new WRBeT::ForceFailure("ForceFailure for " + child->name(), child);
    }

    static inline ForceSuccess *ForceSuccess(Node *child) {
        return new WRBeT::ForceSuccess("ForceSuccess for " + child->name(), child);
    }

    static inline SimpleReturn *SimpleReturn(Node *child) {
        return new WRBeT::SimpleReturn("SimpleReturn for " + child->name(), child);
    }
};
} // namespace WRBeT

#endif // BEHAVIORTREE_HH