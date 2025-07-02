#ifndef SSLCOACH_HH
#define SSLCOACH_HH

#include "world/world.hh"
#include "referee/referee.hh"
#include "agent/sslagent/sslagent.hh"

#include <QVector>

class SSLCoach {    
public:
    SSLCoach(const World &world, const Referee &referee, QVector<SSLAgent *> &agents);

    void delegatePlaysAndRoles();
private:
    const World &world_;
    const Referee &referee_;
    QVector<SSLAgent *> &agents_;

    void setupHalt();
    void setupNaiveOffense();
};

#endif