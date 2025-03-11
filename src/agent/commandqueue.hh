#ifndef COMMANDQUEUE_HH
#define COMMANDQUEUE_HH

#include <QQueue>

#include "agent/basecommand.hh"

namespace Agent {
class CommandQueue {
public:
    CommandQueue();

    void enqueue(BaseCommand *command);
    BaseCommand *dequeue();

    void runAllPendingCommands();

private:
    QQueue<BaseCommand *> commandQueue_;
};
} // namespace Agent

#endif