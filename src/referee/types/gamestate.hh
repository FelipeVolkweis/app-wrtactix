#ifndef GAMESTATE_HH
#define GAMESTATE_HH

#include <QElapsedTimer>
#include <QString>

#include "world/world.hh"

namespace RefereeStates {
enum State {
    HALT,
    TIMEOUT,
    STOP,
    OUR_PREPARE_KICKOFF,
    THEIR_PREPARE_KICKOFF,
    OUR_BALL_PLACEMENT,
    THEIR_BALL_PLACEMENT,
    OUR_PREPARE_PENALTY,
    THEIR_PREPARE_PENALTY,
    OUR_KICKOFF,
    THEIR_KICKOFF,
    OUR_FREEKICK,
    THEIR_FREEKICK,
    OUR_PENALTY,
    THEIR_PENALTY,
    RUN
};
} // namespace RefereeStates

class GameState {
public:
    GameState(const World &world);

    RefereeStates::State state() const {
        return _state;
    }

    QString stateAsString() const;

    void stateMachine(QString refereeCommand);

    bool hasNewState() const {
        return _state != _previousState;
    }

private:
    const World &w() const {
        return _world;
    }

    const World &_world;

    RefereeStates::State halt(QString refereeCommand);
    RefereeStates::State stop(QString refereeCommand);
    RefereeStates::State prepareKickoff(QString refereeCommand);
    RefereeStates::State ballPlacement(QString refereeCommand);
    RefereeStates::State preparePenalty(QString refereeCommand);
    RefereeStates::State kickoff(QString refereeCommand);
    RefereeStates::State freeKick(QString refereeCommand);
    RefereeStates::State penalty(QString refereeCommand);
    RefereeStates::State run(QString refereeCommand);

    RefereeStates::State _state;
    RefereeStates::State _previousState;

    Position _ballPosition;

    QElapsedTimer _kickoffTimer;
    QElapsedTimer _freeKickTimer;
    QElapsedTimer _penaltyTimer;
};
#endif // GAMESTATE_HH