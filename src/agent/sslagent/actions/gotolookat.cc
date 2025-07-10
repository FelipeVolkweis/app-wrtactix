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

GoToLookAt *GoToLookAt::setWaypoint(std::function<Vec2()> waypoint) {
    waypoint_ = waypoint;
    return this;
}

GoToLookAt *GoToLookAt::setSpeedBoost(std::function<float()> speedBoost) {
    speedBoost_ = speedBoost;
    return this;
}

GoToLookAt *GoToLookAt::avoidTeammates(std::function<bool()> condition) {
    avoidTeammatesFn_ = condition;
    if (avoidTeammatesFn_ == nullptr) {
        avoidTeammates_ = true;
    }
    return this;
}

GoToLookAt *GoToLookAt::avoidOpponents(std::function<bool()> condition) {
    avoidOpponentsFn_ = condition;
    if (avoidOpponentsFn_ == nullptr) {
        avoidOpponents_ = true;
    }
    return this;
}

GoToLookAt *GoToLookAt::avoidBall(std::function<bool()> condition) {
    avoidBallFn_ = condition;
    if (avoidBallFn_ == nullptr) {
        avoidBall_ = true;
    }
    return this;
}

GoToLookAt *GoToLookAt::avoidOurGoal(std::function<bool()> condition) {
    avoidOurGoalFn_ = condition;
    if (avoidOurGoalFn_ == nullptr) {
        avoidOurGoal_ = true;
    }
    return this;
}

GoToLookAt *GoToLookAt::avoidTheirGoal(std::function<bool()> condition) {
    avoidTheirGoalFn_ = condition;
    if (avoidTheirGoalFn_ == nullptr) {
        avoidTheirGoal_ = true;
    }
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

    float speedBoost = speedBoost_ ? speedBoost_() : 0.0f;
    Vec2 goal = goal_();
    Vec2 lookAt = lookAt_();
    Vec2 origin = world().playerPositionVec2(id());
    Vec2 direction = lookAt - origin;
    float angle = atan2f(direction.y(), direction.x());
    float orientation = world().playerOrientation(id()).value();

    if ((goal - origin).norm() < 1e-2 && fabsf(angle - orientation) < 0.1) {
        return Status::SUCCESS;
    }

    handleObstacles();
    Vec2 waypoint = waypoint_ ? waypoint_() : goal;

    Vec2 waypointDir = waypoint - origin;
    Vec2 goalDir = goal - origin;

    if ((waypoint - goal).norm() < 0.1) {
        waypoint = goal;
    } else if (goalDir.dot(waypointDir) < 0) {
        waypoint = goal;
    } else if ((waypoint - origin).norm() < 0.1) {
        waypoint = goal;
    }

    auto obs = obstaclesBuilder_.obstacles();
    auto path = pathPlanner_->findPath(origin, waypoint, obs);
    obstaclesBuilder_.reset();

    controller().move(path, lookAt, speedBoost);

    return Status::RUNNING;
}

// If no function was provided, simply avoid the obstacle class, else, evaluate the function
void GoToLookAt::handleObstacles() {
    if ((avoidTeammatesFn_ && avoidTeammatesFn_()) || avoidTeammates_) {
        obstaclesBuilder_.avoidTeammates();
    }
    if ((avoidOpponentsFn_ && avoidOpponentsFn_()) || avoidOpponents_) {
        obstaclesBuilder_.avoidOpponents();
    }
    if ((avoidBallFn_ && avoidBallFn_()) || avoidBall_) {
        obstaclesBuilder_.avoidBall();
    }
    if ((avoidOurGoalFn_ && avoidOurGoalFn_()) || avoidOurGoal_) {
        obstaclesBuilder_.avoidOurGoal();
    }
    if ((avoidTheirGoalFn_ && avoidTheirGoalFn_()) || avoidTheirGoal_) {
        obstaclesBuilder_.avoidTheirGoal();
    }
}