#ifndef COSTNODE_HH
#define COSTNODE_HH

#include "node.hh"

namespace WRBeT {
class CostNode : public Node {
public:
    CostNode(const QString &name);

    float cost() const;
    virtual void calculateCost() = 0;

private:
    float cost_;
};
} // namespace WRBeT

#endif