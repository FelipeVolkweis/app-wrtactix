#include <QLoggingCategory>

#include "parallel.hh"

Q_LOGGING_CATEGORY(PARALLEL, "Parallel")

using namespace WRBeT;

Parallel::Parallel(QString name) : ControlFlowNode(name) {}

Status Parallel::tick() {
    status_ = Status::SUCCESS;
    for (auto &child : _children) {
        qCDebug(PARALLEL) << child->name();
        Status childStatus = child->tick();
        if (childStatus == Status::FAILURE) {
            status_ = Status::FAILURE;
        }
    }
    return status_;
}