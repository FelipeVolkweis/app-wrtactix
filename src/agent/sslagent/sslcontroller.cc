#include "algorithm/geometry/twod/twod.hh"

#include "sslcontroller.hh"

SSLController::SSLController(const PlayerID &id, GEARSystem::Controller &controller, const World &world)
    : id_(id), controller_(controller), linearPid_(0.5, 0.0, 0.0, 1, 1), angularPid_(0.5, 0.0, 0.0, 1, 1), world_(world) {}

void SSLController::move(const QVector<Vec2> &path, const Vec2 &lookAt) {
    if (path.isEmpty()) {
        return;
    }

    Vec2 goal = path.last();
    Vec2 position = TwoD::positionToVector(world_.playerPosition(id_));
    float theta = path.size() > 1 ? TwoD::angleBetweenVectors(path[0], path[1]) : 0.0f;

    Vec2 error = goal - position;
    float distance = error.norm();

    float angleToLookAt = TwoD::angleBetweenVectors(position, lookAt);
    float angleError = angleToLookAt - world_.playerOrientation(id_).value();
    if (angleError > M_PI) {
        angleError -= 2 * M_PI;
    } else if (angleError < -M_PI) {
        angleError += 2 * M_PI;
    }

    float linearSignal = linearPid_.computeSignal(distance);
    float angularSignal = angularPid_.computeSignal(angleError);
    theta = theta - world_.playerOrientation(id_).value() + GEARSystem::Angle::pi / 2;

    float x = linearSignal * cos(theta);
    float y = linearSignal * sin(theta);

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