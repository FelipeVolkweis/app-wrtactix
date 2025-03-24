#include <QLoggingCategory>

#include "sequence.hh"

Q_LOGGING_CATEGORY(SEQUENCE, "Sequence")

using namespace WRBeT;

Sequence::Sequence(QString name) : ControlFlowNode(name) {}

Status Sequence::tick() {
    for (const auto &child : _children) {
        qCDebug(SEQUENCE) << child->name();
        Status childStatus = child->tick();

        if (childStatus == RUNNING) {
            status_ = RUNNING;
            return RUNNING;
        } else if (childStatus == FAILURE) {
            status_ = FAILURE;
            return FAILURE;
        }
    }

    status_ = SUCCESS;
    return SUCCESS;
}