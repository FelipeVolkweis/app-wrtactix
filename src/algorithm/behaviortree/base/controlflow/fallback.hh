#ifndef FALLBACK_HH
#define FALLBACK_HH

#include "controlflownode.hh"

namespace WRBeT {
class Fallback : public ControlFlowNode {
public:
    Fallback(QString name);
    Status tick();
};
} // namespace WRBeT

#endif // FALLBACK_HH