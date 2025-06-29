#ifndef SSLCONTROLLER_HH
#define SSLCONTROLLER_HH

#include <QVector>

#include "algorithm/control/pid/pid.hh"
#include "types/playerid.hh"
#include "types/vec2.hh"
#include "world/world.hh"

#include <GEARSystem/gearsystem.hh>

class SSLController {
public:
    SSLController(const PlayerID &id, GEARSystem::Controller &controller, const World &world);

    void move(const QVector<Vec2> &path, const Vec2 &lookAt);
    void kick();
    void kick(float force);
    void dribble();
    void chipKick();
    void chipKick(float force);
    void stop();

private:
    PlayerID id_;
    GEARSystem::Controller &controller_;
    const World &world_;
    PID linearPid_;
    PID angularPid_;
};

#endif