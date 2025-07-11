#ifndef SIMPLERETURN_HH
#define SIMPLERETURN_HH

#include "algorithm/behaviortree/base/node.hh"

namespace WRBeT {
class SimpleReturn : public Node {
public:
    SimpleReturn(const QString &name, Node *child) : Node(name), _child(child) {}

    Status tick();

private:
    Node *_child;
};
} // namespace WRBeT

#endif