#ifndef PARALLEL_HH
#define PARALLEL_HH

#include "controlflownode.hh"

namespace WRBeT {
class Parallel : public ControlFlowNode {
public:
    Parallel(QString name);
    Status tick();
};
} // namespace WRBeT

#endif // PARALLEL_HH