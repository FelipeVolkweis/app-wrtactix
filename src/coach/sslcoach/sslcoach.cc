#include "sslcoach.hh"
#include "agent/sslagent/plays/plays.hh"

SSLCoach::SSLCoach(const World &world, const Referee &referee, QVector<SSLAgent *> &agents)
    : world_(world), referee_(referee), agents_(agents) {}

void SSLCoach::delegatePlaysAndRoles() {
    if (referee_.hasNewState()) {
        switch (referee_.state()) {
        case RefereeStates::STOP:
        case RefereeStates::HALT:
            setupHalt();
            break;
        default:
            setupNaiveOffense();
            break;
        }
    }
}

void SSLCoach::setupHalt() {
    for (SSLAgent *agent : agents_) {
        agent->setPlay(new PlayHalt(world_));
        agent->setRole(new SSLRole());
    }
}

void SSLCoach::setupNaiveOffense() {
    auto av = world_.ourAvailablePlayers();
    int numPlayers = av.size();

    switch (numPlayers) {
        case 6:
        case 5:
        case 4:
        case 3:
        case 2:
            agents_[av[1].playerNum()]->setPlay(new PlayNaiveOffense(world_));
            agents_[av[1].playerNum()]->setRole(new SSLRole(SSLRoleType::GOALKEEPER));
        case 1:
            agents_[av[0].playerNum()]->setPlay(new PlayNaiveOffense(world_));
            agents_[av[0].playerNum()]->setRole(new SSLRole(SSLRoleType::STRIKER));
        case 0:
            break;
    }
}