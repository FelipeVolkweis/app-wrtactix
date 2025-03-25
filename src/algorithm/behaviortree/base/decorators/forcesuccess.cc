#include <QLoggingCategory>

#include "forcesuccess.hh"

Q_LOGGING_CATEGORY(ALWAYSTRUE, "AlwaysTrue")

using namespace WRBeT;

Status ForceSuccess::tick() {
    _child.tick();
    status_ = Status::SUCCESS;
    return status_;
}