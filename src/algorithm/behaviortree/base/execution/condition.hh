#ifndef CONDITION_HH
#define CONDITION_HH

#include "executionnode.hh"
#include <functional>

namespace WRBeT {
class Condition : public ExecutionNode {
public:
    Condition(QString name, std::function<bool()> conditionTrue);
    Status tick();

private:
    std::function<bool()> _conditionTrue;
};
} // namespace WRBeT
#endif // CONDITION_HH