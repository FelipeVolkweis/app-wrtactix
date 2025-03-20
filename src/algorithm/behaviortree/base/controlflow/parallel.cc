#include <QLoggingCategory>

#include "parallel.hh"

Q_LOGGING_CATEGORY(PARALLEL, "Parallel")

using namespace WRBeT;

Parallel::Parallel(QString name) : ControlFlowNode(name) {}

Status Parallel::tick() {
    Status status = Status::SUCCESS;
    for (auto &child : _children) {
        qCDebug(PARALLEL) << child->name();
        Status childStatus = child->tick();
        if (childStatus == Status::FAILURE) {
            status = Status::FAILURE;
        }
    }
    return status;
}