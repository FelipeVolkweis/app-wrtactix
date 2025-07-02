#include <QElapsedTimer>

#include "agent/sslagent/behaviors/behaviors.hh"

#include "sslagent.hh"

Q_LOGGING_CATEGORY(SSLAGENT, "SSLAgent")

SSLAgent::SSLAgent(PlayerID id, Sides::Side side, GEARSystem::Controller &controller)
    : id_(id), world_(controller), sslController_(id, controller, world_),
      referee_(QHostAddress("224.5.23.1"), 10003, world_), currentBehavior_(nullptr) {
    Colors::Color ourTeamColor = id.teamNum() == 0 ? Colors::YELLOW : Colors::BLUE;
    world_.setColor(ourTeamColor);
    world_.setSide(side);
    referee_.connect();

    goTo_ = (new ShootToGoal(id_, sslController_, world_))->node();
    idle_ = (new DoNothing(id_, sslController_, world_))->node();
    calibrateLinear_ = (new CalibrateLinear(id_, sslController_, world_))->node();
    calibrateAngular_ = (new CalibrateAngular(id_, sslController_, world_))->node();
    goalie_ = (new GoalKeeper(id_, sslController_, world_))->node();

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
            // currentBehavior_ = goTo_;
            // currentBehavior_ = calibrateLinear_;
            // currentBehavior_ = calibrateAngular_;
            currentBehavior_ = goalie_;
            break;
        }
    }
}

void SSLAgent::act() {
    QElapsedTimer timer;
    timer.start();
    if (currentBehavior_) {
        currentBehavior_->tick();
    }

    if (timer.elapsed() > 2) {
        qCInfo(SSLAGENT) << "Behavior tick took" << timer.elapsed() << "ms";
    }

    sslController_.controllerCallback();
}

bool SSLAgent::isActive() const {
    return world_.playerIsActive(id_);
}