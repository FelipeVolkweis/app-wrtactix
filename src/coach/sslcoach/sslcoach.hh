#ifndef SSLCOACH_HH
#define SSLCOACH_HH

#include <QVector>

#include "agent/sslagent/sslagent.hh"
#include "agent/sslagent/sslplay.hh"
#include "referee/referee.hh"
#include "world/world.hh"

class SSLCoach {
public:
    SSLCoach(const World &world, const Referee &referee, QVector<SSLAgent *> &agents);

    void delegatePlaysAndRoles();

private:
    const World &world_;
    const Referee &referee_;
    QVector<SSLAgent *> &agents_;
    SSLPlay *play_ = nullptr;
    QHash<PlayerID, SSLRole *> roleDefinitions_;

    void setupPlay(SSLPlay *play);
};

#endif