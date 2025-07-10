#include <QLoggingCategory>

#include "agent/sslagent/plays/plays.hh"
#include "agent/sslagent/roles/roles.hh"

#include "sslcoach.hh"

Q_LOGGING_CATEGORY(SSLCOACH, "SSLCoach")

SSLCoach::SSLCoach(const World &world, const Referee &referee, QVector<SSLAgent *> &agents)
    : world_(world), referee_(referee), agents_(agents) {}

void SSLCoach::delegatePlaysAndRoles() {
    if (referee_.hasNewState()) {
        delete play_;

        switch (referee_.state()) {
        case RefereeStates::STOP:
        case RefereeStates::HALT:
            play_ = new PlayHalt(world_);
            break;
        default:
            play_ = new PlayNaiveOffense(world_);
            break;
        }

        if (!play_) {
            qCWarning(SSLCOACH) << "Failed to create play for state" << referee_.stateAsString();
            return;
        }

        setupPlay(play_);
    }
}

void SSLCoach::setupPlay(SSLPlay *play) {
    QHash<PlayerID, bool> assignments;
    const auto &avPlayers = world_.ourAvailablePlayers();
    for (const auto &avPlayer : avPlayers) {
        assignments[avPlayer] = false;
        agents_[avPlayer.playerNum()]->setPlay(play);
    }

    auto &suggestions = play->getSuggestedAssignments();
    int i = 0;

    if (suggestions.isEmpty()) {
        qCWarning(SSLCOACH) << "No suggested assignments found for play";
        return;
    }

    if (suggestions.size() < avPlayers.size()) {
        qCWarning(SSLCOACH) << "Fewer suggested assignments than available agents";
    }

    for (const auto &suggestion : suggestions) {
        const auto &suggestedRole = suggestion.second;
        SSLRole *role = nullptr;
        switch (suggestedRole) {
        case SSLRoleType::GOALKEEPER:
            role = new SSLGoalkeeper();
            break;
        case SSLRoleType::STRIKER:
            role = new SSLStriker();
            break;
        case SSLRoleType::BARRIERRIGHT:
            role = new SSLBarrierRight();
            break;
        case SSLRoleType::BARRIERLEFT:
            role = new SSLBarrierLeft();
            break;
        case SSLRoleType::BARRIERMIDDLE:
            role = new SSLBarrierMiddle();
            break;
        default:
            continue;
        }

        if (!role)
            continue;
        auto id = role->getRoleAssignment(world_, assignments);
        if (id.isValid()) {
            agents_[id.playerNum()]->setRole(role);
            assignments[id] = true;
        } else {
            delete role;
        }
    }
}