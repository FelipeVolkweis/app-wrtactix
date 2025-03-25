#include <QLoggingCategory>

#include "sequence.hh"

Q_LOGGING_CATEGORY(SEQUENCE, "Sequence")

using namespace WRBeT;

Sequence::Sequence(QString name) : ControlFlowNode(name) {}

Status Sequence::tick() {
    for (const auto &child : _children) {
        qCDebug(SEQUENCE) << child->name();
        Status childStatus = child->tick();

        if (childStatus == Status::RUNNING) {
            status_ = Status::RUNNING;
            return status_;
        } else if (childStatus == Status::FAILURE) {
            status_ = Status::FAILURE;
            return status_;
        }
    }

    status_ = Status::SUCCESS;
    return status_;
}