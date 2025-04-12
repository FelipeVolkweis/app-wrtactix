#include "costnode.hh"

using namespace WRBeT;

CostNode::CostNode(const QString &name) : Node(name), cost_(INFINITY) {}

float CostNode::cost() const {
    return cost_;
}