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

        if (childStatus == RUNNING) {
            status_ = RUNNING;
            return RUNNING;
        } else if (childStatus == SUCCESS) {
            status_ = SUCCESS;
            return SUCCESS;
        }
    }

    status_ = FAILURE;
    return FAILURE;
}