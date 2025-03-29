#include "algorithm/geometry/twod/twod.hh"

#include "gamestate.hh"

GameState::GameState(const World &world) : _world(world) {
    _state = RefereeStates::HALT;
}

void GameState::stateMachine(QString refereeCommand) {
    _previousState = _state;
    if (refereeCommand == "HALT") {
        _state = RefereeStates::HALT;
        return;
    }
    switch (_state) {
    case RefereeStates::HALT:
        _state = halt(refereeCommand);
        break;
    case RefereeStates::STOP:
        _state = stop(refereeCommand);
        break;
    case RefereeStates::OUR_PREPARE_KICKOFF:
    case RefereeStates::THEIR_PREPARE_KICKOFF:
        _state = prepareKickoff(refereeCommand);
        break;
    case RefereeStates::OUR_BALL_PLACEMENT:
    case RefereeStates::THEIR_BALL_PLACEMENT:
        _state = ballPlacement(refereeCommand);
        break;
    case RefereeStates::OUR_PREPARE_PENALTY:
    case RefereeStates::THEIR_PREPARE_PENALTY:
        _state = preparePenalty(refereeCommand);
        break;
    case RefereeStates::OUR_KICKOFF:
    case RefereeStates::THEIR_KICKOFF:
        _state = kickoff(refereeCommand);
        break;
    case RefereeStates::OUR_FREEKICK:
    case RefereeStates::THEIR_FREEKICK:
        _state = freeKick(refereeCommand);
        break;
    case RefereeStates::OUR_PENALTY:
    case RefereeStates::THEIR_PENALTY:
        _state = penalty(refereeCommand);
        break;
    case RefereeStates::RUN:
        _state = run(refereeCommand);
        break;
    default:
        _state = RefereeStates::HALT;
    }
}

RefereeStates::State GameState::halt(QString refereeCommand) {
    if (refereeCommand == "STOP") {
        return RefereeStates::STOP;
    }
    if (refereeCommand == "NORMAL_START") {
        return RefereeStates::RUN;
    }
    if (refereeCommand == "FORCE_START") {
        return RefereeStates::RUN;
    }
    return _state;
}

RefereeStates::State GameState::stop(QString refereeCommand) {
    if (refereeCommand == "FORCE_START") {
        return RefereeStates::RUN;
    }
    if (refereeCommand == "PREPARE_KICKOFF_YELLOW") {
        if (w().ourColor() == Colors::YELLOW) {
            return RefereeStates::OUR_PREPARE_KICKOFF;
        } else {
            return RefereeStates::THEIR_PREPARE_KICKOFF;
        }
    }
    if (refereeCommand == "PREPARE_KICKOFF_BLUE") {
        if (w().ourColor() == Colors::BLUE) {
            return RefereeStates::OUR_PREPARE_KICKOFF;
        } else {
            return RefereeStates::THEIR_PREPARE_KICKOFF;
        }
    }
    if (refereeCommand == "BALL_PLACEMENT_YELLOW") {
        if (w().ourColor() == Colors::YELLOW) {
            return RefereeStates::OUR_BALL_PLACEMENT;
        } else {
            return RefereeStates::THEIR_BALL_PLACEMENT;
        }
    }
    if (refereeCommand == "BALL_PLACEMENT_BLUE") {
        if (w().ourColor() == Colors::BLUE) {
            return RefereeStates::OUR_BALL_PLACEMENT;
        } else {
            return RefereeStates::THEIR_BALL_PLACEMENT;
        }
    }
    if (refereeCommand == "PREPARE_PENALTY_YELLOW") {
        if (w().ourColor() == Colors::YELLOW) {
            return RefereeStates::OUR_PREPARE_PENALTY;
        } else {
            return RefereeStates::THEIR_PREPARE_PENALTY;
        }
    }
    if (refereeCommand == "PREPARE_PENALTY_BLUE") {
        if (w().ourColor() == Colors::BLUE) {
            return RefereeStates::OUR_PREPARE_PENALTY;
        } else {
            return RefereeStates::THEIR_PREPARE_PENALTY;
        }
    }
    if (refereeCommand == "DIRECT_FREE_YELLOW") {
        _freeKickTimer.start();
        _ballPosition = w().ballPosition();
        if (w().ourColor() == Colors::YELLOW) {
            return RefereeStates::OUR_FREEKICK;
        } else {
            return RefereeStates::THEIR_FREEKICK;
        }
    }
    if (refereeCommand == "DIRECT_FREE_BLUE") {
        _freeKickTimer.start();
        _ballPosition = w().ballPosition();
        if (w().ourColor() == Colors::BLUE) {
            return RefereeStates::OUR_FREEKICK;
        } else {
            return RefereeStates::THEIR_FREEKICK;
        }
    }
    return _state;
}

RefereeStates::State GameState::prepareKickoff(QString refereeCommand) {
    if (refereeCommand == "NORMAL_START") {
        _kickoffTimer.start();
        _ballPosition = w().ballPosition();
        if (_state == RefereeStates::OUR_PREPARE_KICKOFF) {
            return RefereeStates::OUR_KICKOFF;
        }
        if (_state == RefereeStates::THEIR_PREPARE_KICKOFF) {
            return RefereeStates::THEIR_KICKOFF;
        }
    }
    return _state;
}

RefereeStates::State GameState::ballPlacement(QString refereeCommand) {
    if (refereeCommand == "STOP") {
        return RefereeStates::STOP;
    }
    return _state;
}

RefereeStates::State GameState::preparePenalty(QString refereeCommand) {
    if (refereeCommand == "NORMAL_START") {
        _penaltyTimer.start();
        if (_state == RefereeStates::OUR_PREPARE_PENALTY) {
            return RefereeStates::OUR_PENALTY;
        }
        if (_state == RefereeStates::THEIR_PREPARE_PENALTY) {
            return RefereeStates::THEIR_PENALTY;
        }
    }
    return _state;
}

RefereeStates::State GameState::kickoff(QString refereeCommand) {
    int elapsed = _kickoffTimer.elapsed();
    float ballDistance = TwoD::distance(_ballPosition, w().ballPosition());
    if (elapsed > 10 * 1000) {
        return RefereeStates::RUN;
    }
    if (ballDistance > 0.1) {
        return RefereeStates::RUN;
    }
    return _state;
}

RefereeStates::State GameState::freeKick(QString refereeCommand) {
    int elapsed = _freeKickTimer.elapsed();
    float ballDistance = TwoD::distance(_ballPosition, w().ballPosition());
    if (elapsed > 10 * 1000) {
        return RefereeStates::RUN;
    }
    if (ballDistance > 0.1) {
        return RefereeStates::RUN;
    }
    return _state;
}

RefereeStates::State GameState::penalty(QString refereeCommand) {
    int elapsed = _penaltyTimer.elapsed();
    if (elapsed > 10 * 1000) {
        return RefereeStates::STOP;
    }
    if (refereeCommand == "STOP") {
        return RefereeStates::STOP;
    }
    return _state;
}

RefereeStates::State GameState::run(QString refereeCommand) {
    if (refereeCommand == "STOP") {
        return RefereeStates::STOP;
    }
    return _state;
}

QString GameState::stateAsString() const {
    switch (_state) {
    case RefereeStates::HALT:
        return "HALT";
    case RefereeStates::STOP:
        return "STOP";
    case RefereeStates::OUR_PREPARE_KICKOFF:
        return "OUR_PREPARE_KICKOFF";
    case RefereeStates::THEIR_PREPARE_KICKOFF:
        return "THEIR_PREPARE_KICKOFF";
    case RefereeStates::OUR_BALL_PLACEMENT:
        return "OUR_BALL_PLACEMENT";
    case RefereeStates::THEIR_BALL_PLACEMENT:
        return "THEIR_BALL_PLACEMENT";
    case RefereeStates::OUR_PREPARE_PENALTY:
        return "OUR_PREPARE_PENALTY";
    case RefereeStates::THEIR_PREPARE_PENALTY:
        return "THEIR_PREPARE_PENALTY";
    case RefereeStates::OUR_KICKOFF:
        return "OUR_KICKOFF";
    case RefereeStates::THEIR_KICKOFF:
        return "THEIR_KICKOFF";
    case RefereeStates::OUR_FREEKICK:
        return "OUR_FREEKICK";
    case RefereeStates::THEIR_FREEKICK:
        return "THEIR_FREEKICK";
    case RefereeStates::OUR_PENALTY:
        return "OUR_PENALTY";
    case RefereeStates::THEIR_PENALTY:
        return "THEIR_PENALTY";
    case RefereeStates::RUN:
        return "RUN";
    default:
        return "UNKNOWN";
    }
}