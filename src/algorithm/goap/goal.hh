#ifndef GOAL_HH
#define GOAL_HH

#include <QHash>
#include <QString>

#include "types/dictionary.hh"

class Goal {
public:
    Goal(const QString &name) : name_(name) {};

    virtual bool isValid() const = 0;
    virtual int priority() const = 0;
    virtual Dictionary<bool> desiredWorldState() const = 0;

    const QString &name() const {
        return name_;
    }

private:
    const QString name_;
};

#endif