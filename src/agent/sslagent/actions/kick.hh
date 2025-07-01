#ifndef KICK_HH
#define KICK_HH

#include "agent/sslagent/sslaction.hh"

class Kick : public SSLAction {
public:
    Kick(const PlayerID &player, SSLController &controller, const World &world);

    Kick *setPower(float power);

    Status execute();

private:
    float power_;
};

#endif