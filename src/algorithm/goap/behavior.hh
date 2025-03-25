#ifndef BEHAVIOR_HH
#define BEHAVIOR_HH

#include "types/dictionary.hh"
#include "types/status.hh"

class Behavior {
public:
    Behavior(const QString &name) : name_(name) {}

    virtual bool isValid() const = 0;
    virtual float cost() const = 0;
    virtual Dictionary<bool> preconditions() const = 0;
    virtual Dictionary<bool> effects() const = 0;
    virtual Status execute() const = 0;

    const QString &name() const {
        return name_;
    }

private:
    const QString name_;
};

#endif