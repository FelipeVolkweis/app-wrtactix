#ifndef ALWAYS_TRUE
#define ALWAYS_TRUE

#include "algorithm/behaviortree/base/node.hh"

namespace WRBeT {
class ForceSuccess : public Node {
public:
    ForceSuccess(const QString &name, Node &child) : Node(name), _child(child) {}

    Status tick();

private:
    Node &_child;
};
} // namespace WRBeT

#endif // ALWAYS_TRUE