#ifndef CONTROLNODE_HH
#define CONTROLNODE_HH

#include <QVector>

#include "algorithm/behaviortree/node.hh"

namespace WRBeT {
class ControlFlowNode : public Node {
public:
    ControlFlowNode(QString name);
    void addChild(Node *child);

protected:
    QVector<Node *> _children;
};
} // namespace WRBeT

#endif // CONTROLNODE_HH