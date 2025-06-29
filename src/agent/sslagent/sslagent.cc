#include "sslagent.hh"
#include "agent/sslagent/behaviors/donothing.hh"
#include "agent/sslagent/behaviors/shoottogoal.hh"

SSLAgent::SSLAgent(PlayerID id, Sides::Side side, GEARSystem::Controller &controller)
    : id_(id), world_(controller), sslController_(id, controller, world_), referee_(QHostAddress("224.5.23.1"), 10003, world_),
      currentBehavior_(nullptr) {
    Colors::Color ourTeamColor = id.teamNum() == 0 ? Colors::YELLOW : Colors::BLUE;
    world_.setColor(ourTeamColor);
    world_.setSide(side);
    referee_.connect();

    goTo_ = (new ShootToGoal(id_, sslController_, world_))->node();
    idle_ = (new DoNothing(id_, sslController_, world_))->node();
    currentBehavior_ = idle_;
}

void SSLAgent::observe() {
    world_.update();
}

void SSLAgent::listen() {
    referee_.bufferize();
    referee_.update();
}

void SSLAgent::think() {
    if (referee_.hasNewState()) {
        switch (referee_.state()) {
        case RefereeStates::STOP:
        case RefereeStates::HALT:
            currentBehavior_ = idle_;
            break;
        default:
            currentBehavior_ = goTo_;
            break;
        }
    }
}

void SSLAgent::act() {
    if (currentBehavior_) {
        currentBehavior_->tick();
    }
}

bool SSLAgent::isActive() const {
    return world_.playerIsActive(id_);
}