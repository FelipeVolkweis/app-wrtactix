#ifndef EXECUTIONNODE_HH
#define EXECUTIONNODE_HH

#include "algorithm/behaviortree/node.hh"

namespace WRBeT {
class ExecutionNode : public Node {
public:
    ExecutionNode(QString name);
    virtual Status tick() = 0;
};
} // namespace WRBeT

#endif // EXECUTIONNODE_HH