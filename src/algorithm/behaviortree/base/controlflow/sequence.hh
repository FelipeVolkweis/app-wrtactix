#ifndef SEQUENCE_HH
#define SEQUENCE_HH

#include "controlflownode.hh"

namespace WRBeT {
class Sequence : public ControlFlowNode {
public:
    Sequence(QString name);
    Status tick();
};
} // namespace WRBeT

#endif // SEQUENCE_HH