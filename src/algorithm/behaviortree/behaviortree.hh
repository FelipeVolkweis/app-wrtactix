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
};
} // namespace WRBeT

#endif // BEHAVIORTREE_HH