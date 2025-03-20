#include "action.hh"

using namespace WRBeT;

Action::Action(QString name) : ExecutionNode(name) {}

Status Action::tick() {
    return execute();
}