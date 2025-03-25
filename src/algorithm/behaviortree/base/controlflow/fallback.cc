#include <QLoggingCategory>

#include "fallback.hh"

Q_LOGGING_CATEGORY(FALLBACK, "Fallback")

using namespace WRBeT;

Fallback::Fallback(QString name) : ControlFlowNode(name) {}

Status Fallback::tick() {
    for (const auto &child : _children) {
        qCDebug(FALLBACK) << child->name();
        Status childStatus = child->tick();
        status_ = childStatus;

        if (childStatus == Status::RUNNING) {
            status_ = Status::RUNNING;
            return status_;
        } else if (childStatus == Status::SUCCESS) {
            status_ = Status::SUCCESS;
            return status_;
        }
    }

    status_ = Status::FAILURE;
    return status_;
}