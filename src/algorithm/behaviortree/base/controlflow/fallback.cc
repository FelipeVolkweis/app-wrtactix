#include <QLoggingCategory>

#include "fallback.hh"

Q_LOGGING_CATEGORY(FALLBACK, "Fallback")

using namespace WRBeT;

Fallback::Fallback(QString name) : ControlFlowNode(name) {}

Status Fallback::tick() {
    for (const auto &child : _children) {
        qCDebug(FALLBACK) << child->name();
        Status childStatus = child->tick();

        if (childStatus == RUNNING) {
            return RUNNING;
        } else if (childStatus == SUCCESS) {
            return SUCCESS;
        }
    }

    return FAILURE;
}