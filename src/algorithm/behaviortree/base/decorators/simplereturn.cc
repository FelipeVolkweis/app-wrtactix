#include <QLoggingCategory>

#include "simplereturn.hh"

Q_LOGGING_CATEGORY(SIMPLERETURN, "SimpleReturn")

using namespace WRBeT;

Status SimpleReturn::tick() {
    return _child->tick();
}