#ifndef BASECOMMAND_HH
#define BASECOMMAND_HH

#include <QMutex>

namespace Agent {
class BaseCommand {
public:
    enum class Status { PENDING, RUNNING, FINISHED };

    BaseCommand();

    void run();
    virtual void execute() = 0;

    Status status() const;
    void setStatus(Status status);

private:
    Status status_;
    QMutex statusMutex_;
};
} // namespace Agent

#endif