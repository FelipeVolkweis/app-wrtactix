#include "algorithm/geometry/twod/twod.hh"
#include "constants/constants.hh"

#include "sslcontroller.hh"

SSLController::SSLController(const PlayerID &id, GEARSystem::Controller &controller, const World &world)
    : id_(id), controller_(controller), world_(world),
      linearPid_(Const::Control::PID::linear.kp, Const::Control::PID::linear.ki, Const::Control::PID::linear.kd,
                 Const::Control::Timing::max_dt, Const::Control::PID::linear.max_integral),
      angularPid_(Const::Control::PID::angular.kp, Const::Control::PID::angular.ki, Const::Control::PID::angular.kd,
                  Const::Control::Timing::max_dt, Const::Control::PID::angular.max_integral),
      accelerationRamp_(Const::Control::Movement::max_linear_accel, Const::Control::Movement::max_linear_speed,
                        Const::Control::Timing::max_dt) {}

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

    Vec2 targetVelocity(x, y);
    if (!lastTargetVelocity_.isZero()) {
        targetVelocity = accelerationRamp_.fitSpeedToRamp(lastTargetVelocity_, targetVelocity);
    }
    lastTargetVelocity_ = targetVelocity;
    controller_.setSpeed(id_.teamNum(), id_.playerNum(), targetVelocity.x(), targetVelocity.y(), angularSignal);
}

void SSLController::kick() {
    kick(1.0);
}

void SSLController::kick(float power) {
    controller_.kickOnTouch(id_.teamNum(), id_.playerNum(), true, power);
    kickEnabledTimer_.start();
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

void SSLController::controllerCallback() {
    if (kickEnabledTimer_.isValid() && kickEnabledTimer_.elapsed() > Const::Control::Timing::kick_enabled_timeout) {
        controller_.kickOnTouch(id_.teamNum(), id_.playerNum(), false, 0);
        kickEnabledTimer_.invalidate();
    }
}