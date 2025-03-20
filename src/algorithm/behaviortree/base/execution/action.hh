#ifndef ACTION_HH
#define ACTION_HH

#include "executionnode.hh"

namespace WRBeT {
class Action : public ExecutionNode {
public:
    Action(QString name);
    Status tick();
    virtual Status execute() = 0;

protected:
};
} // namespace WRBeT

#endif // ACTION_HH