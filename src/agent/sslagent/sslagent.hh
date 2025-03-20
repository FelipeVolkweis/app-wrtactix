#ifndef SSLAGENT_HH
#define SSLAGENT_HH

#include <QQueue>

#include "agent/baseagent.hh"
#include "agent/basecommand.hh"
#include "types/playerid.hh"

namespace Agent {
class SSLAgent : BaseAgent {
public:
    SSLAgent(PlayerID id);

    void observe();
    void listen();
    void think();
    void act();

private:
    PlayerID id_;

    QQueue<BaseCommand *> commands_;
};
} // namespace Agent

#endif