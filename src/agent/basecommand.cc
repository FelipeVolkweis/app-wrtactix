#include <QMutexLocker>

#include "basecommand.hh"

using namespace Agent;

BaseCommand::BaseCommand() : status_(Status::PENDING) {}

void BaseCommand::run() {
    {
        QMutexLocker locker(&statusMutex_);
        status_ = Status::RUNNING;
    }
    execute();
    {
        QMutexLocker locker(&statusMutex_);
        status_ = Status::FINISHED;
    }
}

BaseCommand::Status BaseCommand::status() const {
    return status_;
}

void BaseCommand::setStatus(Status status) {
    QMutexLocker locker(&statusMutex_);
    status_ = status;
}