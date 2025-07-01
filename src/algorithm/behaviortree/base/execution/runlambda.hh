#ifndef RUNLAMBDA_HH
#define RUNLAMBDA_HH

#include "executionnode.hh"
#include <functional>

namespace WRBeT {
class RunLambda : public ExecutionNode {
public:
    RunLambda(QString name, std::function<void()> lambda);
    Status tick();

private:
    std::function<void()> lambda_;
};
} // namespace WRBeT

#endif