#include <QLoggingCategory>

#include "agent/wrappers/obstaclesbuilder.hh"

#include "gotolookat.hh"

Q_LOGGING_CATEGORY(GOTOLOOKAT, "GoToLookAt")

GoToLookAt::GoToLookAt(const PlayerID &player, SSLController &controller, const World &world)
    : SSLAction(player, controller, world, "GoToLookAt"), pathPlanner_(nullptr), obstaclesBuilder_(world, player) {}

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

GoToLookAt *GoToLookAt::avoidTeammates(std::function<bool()> condition) {
    avoidTeammates_ = condition;
    return this;
}

GoToLookAt *GoToLookAt::avoidOpponents(std::function<bool()> condition) {
    avoidOpponents_ = condition;
    return this;
}

GoToLookAt *GoToLookAt::avoidBall(std::function<bool()> condition) {
    avoidBall_ = condition;
    return this;
}

GoToLookAt *GoToLookAt::avoidOurGoal(std::function<bool()> condition) {
    avoidOurGoal_ = condition;
    return this;
}

GoToLookAt *GoToLookAt::avoidTheirGoal(std::function<bool()> condition) {
    avoidTheirGoal_ = condition;
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
    // qCInfo(GOTOLOOKAT) << "Origin:" << vec2ToString(origin) << "Goal:" << vec2ToString(goal) << "LookAt:" <<
    // vec2ToString(lookAt)
    //                  << "Direction:" << vec2ToString(direction);
    float angle = atan2f(direction.y(), direction.x());
    float orientation = world().playerOrientation(id()).value();

    if ((goal - origin).norm() < 1e-4 && fabsf(angle - orientation) < 0.1) {
        return Status::SUCCESS;
    }

    handleObstacles();
    auto obs = obstaclesBuilder_.obstacles();
    auto path = pathPlanner_->findPath(origin, goal, obs);
    obstaclesBuilder_.reset();

    controller().move(path, lookAt);

    return Status::RUNNING;
}

// If no function was provided, simply avoid the obstacle class, else, evaluate the function
void GoToLookAt::handleObstacles() {
    if (!avoidTeammates_ || avoidTeammates_()) {
        obstaclesBuilder_.avoidTeammates();
    }
    if (!avoidOpponents_ || avoidOpponents_()) {
        obstaclesBuilder_.avoidOpponents();
    }
    if (!avoidBall_ || avoidBall_()) {
        qWarning() << "Avoiding ball";
        obstaclesBuilder_.avoidBall();
    }
    if (!avoidOurGoal_ || avoidOurGoal_()) {
        obstaclesBuilder_.avoidOurGoal();
    }
    if (!avoidTheirGoal_ || avoidTheirGoal_()) {
        obstaclesBuilder_.avoidTheirGoal();
    }
}