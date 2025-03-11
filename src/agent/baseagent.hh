#ifndef BASEAGENT_HH
#define BASEAGENT_HH

#include "types/playerid.hh"

namespace Agent {
class BaseAgent {
public:
    BaseAgent() {};

    virtual void observe() = 0;
    virtual void listen() = 0;
    virtual void think() = 0;
    virtual void act() = 0;
};
} // namespace Agent

#endif