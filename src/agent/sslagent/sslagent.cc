#include <QElapsedTimer>

#include "agent/sslagent/behaviors/behaviors.hh"

#include "sslagent.hh"

Q_LOGGING_CATEGORY(SSLAGENT, "SSLAgent")

SSLAgent::SSLAgent(PlayerID id, Sides::Side side, GEARSystem::Controller &controller, const World &world,
                   const Referee &referee)
    : id_(id), world_(world), sslController_(id, controller, world_), referee_(referee), currentBehavior_(nullptr),
      play_(nullptr), role_(nullptr) {
    Colors::Color ourTeamColor = id.teamNum() == 0 ? Colors::YELLOW : Colors::BLUE;

    calibrateLinear_ = (new CalibrateLinear(id_, sslController_, world_));
    calibrateAngular_ = (new CalibrateAngular(id_, sslController_, world_));
}

void SSLAgent::setPlay(SSLPlay *play) {
    play_ = play;
    isNewPlay_ = true;
}

void SSLAgent::setRole(SSLRole *role) {
    delete role_;
    role_ = role;
}

void SSLAgent::observe() {}

void SSLAgent::listen() {}

void SSLAgent::think() {
    if (play_ && role_) {
        if (!isNewPlay_) return;
        auto bhv = play_->getBehavior(*role_, id_, sslController_);

        delete currentBehavior_;
        currentBehavior_ = bhv;
        isNewPlay_ = false;
    }
}

void SSLAgent::act() {
    QElapsedTimer timer;
    timer.start();
    if (currentBehavior_) {
        // qCInfo(SSLAGENT) << "Acting for player" << id_.playerNum();
        currentBehavior_->root()->tick();
    }

    if (timer.elapsed() > 2) {
        qCInfo(SSLAGENT) << "Behavior tick took" << timer.elapsed() << "ms";
    }

    sslController_.controllerCallback();
}

bool SSLAgent::isActive() const {
    return world_.playerIsActive(id_);
}