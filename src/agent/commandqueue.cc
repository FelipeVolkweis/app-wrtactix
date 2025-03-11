#include <QFuture>
#include <QtConcurrent>

#include "commandqueue.hh"

using namespace Agent;

CommandQueue::CommandQueue() {}

void CommandQueue::enqueue(BaseCommand *command) {
    command->setStatus(BaseCommand::Status::PENDING);
    commandQueue_.enqueue(command);
}

BaseCommand *CommandQueue::dequeue() {
    return commandQueue_.dequeue();
}

void CommandQueue::runAllPendingCommands() {
    while (!commandQueue_.isEmpty()) {
        BaseCommand *command = commandQueue_.dequeue();

        if (command->status() == BaseCommand::Status::PENDING) {
            QtConcurrent::run([command]() { command->run(); });
        }
    }
}