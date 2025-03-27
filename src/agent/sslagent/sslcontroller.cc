#include "algorithm/geometry/twod/twod.hh"

#include "sslcontroller.hh"

SSLController::SSLController(const PlayerID &id, GEARSystem::Controller &controller)
    : id_(id), controller_(controller), linearPid_(0.5, 0.0, 0.0, 1, 1), angularPid_(0.5, 0.0, 0.0, 1, 1) {}

void SSLController::move(const QVector<Vec2> &path, float angle) {
    if (path.isEmpty()) {
        return;
    }

    Vec2 goal = path.last();
    Vec2 position = TwoD::positionToVector(controller_.playerPosition(id_.teamNum(), id_.playerNum()));

    Vec2 error = goal - position;
    float distance = error.norm();

    float angleError = angle - controller_.playerOrientation(id_.teamNum(), id_.playerNum()).value();
    if (angleError > M_PI) {
        angleError -= 2 * M_PI;
    } else if (angleError < -M_PI) {
        angleError += 2 * M_PI;
    }

    float linearSignal = linearPid_.computeSignal(distance);
    float angularSignal = angularPid_.computeSignal(angleError);

    float x = linearSignal * cos(angleError);
    float y = linearSignal * sin(angleError);

    controller_.setSpeed(id_.teamNum(), id_.playerNum(), x, y, angularSignal);
}

void SSLController::kick() {
    kick(1.0);
}

void SSLController::kick(float power) {
    controller_.kickOnTouch(id_.teamNum(), id_.playerNum(), true, power);
}

void SSLController::dribble() {}

void SSLController::chipKick() {
    chipKick(1.0);
}

void SSLController::chipKick(float power) {
    controller_.chipKick(id_.teamNum(), id_.playerNum(), power);
}

void SSLController::stop() {
    controller_.setSpeed(id_.teamNum(), id_.playerNum(), 0, 0, 0);
    controller_.kickOnTouch(id_.teamNum(), id_.playerNum(), false, 0);
    controller_.chipKick(id_.teamNum(), id_.playerNum(), 0);
}