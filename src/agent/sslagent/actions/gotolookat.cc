#include <QLoggingCategory>
#include "gotolookat.hh"

Q_LOGGING_CATEGORY(GOTOLOOKAT, "GoToLookAt")

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
    qCInfo(GOTOLOOKAT) << "Executing GoToLookAt action";
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
    // qCInfo(GOTOLOOKAT) << "Origin:" << vec2ToString(origin) << "Goal:" << vec2ToString(goal) << "LookAt:" << vec2ToString(lookAt)
    //                  << "Direction:" << vec2ToString(direction);
    float angle = atan2f(direction.y(), direction.x());
    float orientation = world().playerOrientation(id()).value();

    if ((goal - origin).norm() < 1e-4 && fabsf(angle - orientation) < 0.1) {
        return Status::SUCCESS;
    }

    QVector<Obstacle> obs;
    auto path = pathPlanner_->findPath(origin, goal, obs);

    controller().move(path, lookAt);

    return Status::RUNNING;
}