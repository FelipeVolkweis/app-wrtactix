#ifndef FORCEFAILURE_HH
#define FORCEFAILURE_HH

#include "algorithm/behaviortree/base/node.hh"

namespace WRBeT {
class ForceFailure : public Node {
public:
    ForceFailure(const QString &name, Node &child) : Node(name), _child(child) {}

    Status tick();

private:
    Node &_child;
};
} // namespace WRBeT
#endif