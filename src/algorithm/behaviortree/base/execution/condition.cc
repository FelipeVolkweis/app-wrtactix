#include "condition.hh"

using namespace WRBeT;

Condition::Condition(QString name, std::function<bool()> conditionTrue)
    : ExecutionNode(name), _conditionTrue(conditionTrue) {}

Status Condition::tick() {
    if (_conditionTrue()) {
        status_ = SUCCESS;
        return SUCCESS;
    }
    status_ = FAILURE;
    return FAILURE;
}