#ifndef LOOKAT_HH
#define LOOKAT_HH

#include "agent/sslagent/sslaction.hh"

#include <functional>

class LookAt : public SSLAction {
public:
    LookAt(const PlayerID &player, SSLController &controller, const World &world);

    LookAt *setLookAt(std::function<Vec2()> target);

    Status execute();

private:
    std::function<Vec2()> target_;
};

#endif