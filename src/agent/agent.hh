#ifndef AGENT_HH
#define AGENT_HH

class Agent {
    virtual void observe() = 0;
    virtual void listen() = 0;
    virtual void think() = 0;
    virtual void act() = 0;
};

#endif