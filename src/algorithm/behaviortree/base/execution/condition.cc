#include "condition.hh"

using namespace WRBeT;

Condition::Condition(QString name, std::function<bool()> conditionTrue)
    : ExecutionNode(name), _conditionTrue(conditionTrue) {}

Status Condition::tick() {
    if (_conditionTrue()) {
        status_ = Status::SUCCESS;
        return status_;
    }
    status_ = Status::FAILURE;
    return status_;
}