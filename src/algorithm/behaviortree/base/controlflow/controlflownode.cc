#include "controlflownode.hh"

using namespace WRBeT;

ControlFlowNode::ControlFlowNode(QString name) : Node(name) {}

void ControlFlowNode::addChild(Node *child) {
    child->assignParent(this);
    _children.push_back(child);
}