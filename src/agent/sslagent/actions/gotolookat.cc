#include "gotolookat.hh"

GoToLookAt::GoToLookAt(const PlayerID &player, SSLController &controller, const World &world)
    : SSLAction(player, controller, world, "GoToLookAt"), pathPlanner_(nullptr) {}

GoToLookAt *GoToLookAt::setGoal(std::function<Vec2()> goal) {
    goal_ = goal;
    return this;
}

GoToLookAt *GoToLookAt::setPathPlanner(PathPlanner *pathPlanner) {
    pathPlanner_ = pathPlanner;
    return this;
}

GoToLookAt *GoToLookAt::setLookAt(std::function<Vec2()> lookAt) {
    lookAt_ = lookAt;
    return this;
}

Status GoToLookAt::execute() {
    if (!goal_) {
        return Status::FAILURE;
    }

    if (!pathPlanner_) {
        return Status::FAILURE;
    }

    if (!lookAt_) {
        return Status::FAILURE;
    }

    Vec2 goal = goal_();
    Vec2 lookAt = lookAt_();
    Vec2 origin = world().playerPositionVec2(id());
    Vec2 direction = lookAt - origin;
    float angle = atan2f(direction.y(), direction.x());
    float orientation = world().playerOrientation(id()).value();

    if ((goal - origin).norm() < 1e-4 && fabsf(angle - orientation) < 0.1) {
        return Status::SUCCESS;
    }

    QVector<Obstacle> obs;
    auto path = pathPlanner_->findPath(origin, goal, obs);

    controller().move(path, angle);

    return Status::RUNNING;
}