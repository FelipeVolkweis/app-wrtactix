#include <QLoggingCategory>

#include "forcefailure.hh"

Q_LOGGING_CATEGORY(FORCEFAILURE, "ForceFailure")

using namespace WRBeT;

Status ForceFailure::tick() {
    _child.tick();

    return Status::FAILURE;
}