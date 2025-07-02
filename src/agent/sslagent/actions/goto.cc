#include "algorithm/geometry/twod/twod.hh"

#include "goto.hh"

using namespace WRBeT;

GoTo::GoTo(const PlayerID &player, SSLController &controller, const World &world)
    : pathPlanner_(nullptr), SSLAction(player, controller, world, "GoTo"), obstaclesBuilder_(world, player) {}

GoTo *GoTo::setGoal(std::function<Vec2()> goal) {
    goal_ = goal;
    return this;
}

GoTo *GoTo::setPathPlanner(PathPlanner *pathPlanner) {
    pathPlanner_ = pathPlanner;
    return this;
}

GoTo *GoTo::avoidTeammates(std::function<bool()> condition) {
    avoidTeammatesFn_ = condition;
    if (avoidTeammatesFn_ == nullptr) {
        avoidTeammates_ = true;
    }
    return this;
}

GoTo *GoTo::avoidOpponents(std::function<bool()> condition) {
    avoidOpponentsFn_ = condition;
    if (avoidOpponentsFn_ == nullptr) {
        avoidOpponents_ = true;
    }
    return this;
}

GoTo *GoTo::avoidBall(std::function<bool()> condition) {
    avoidBallFn_ = condition;
    if (avoidBallFn_ == nullptr) {
        avoidBall_ = true;
    }
    return this;
}

GoTo *GoTo::avoidOurGoal(std::function<bool()> condition) {
    avoidOurGoalFn_ = condition;
    if (avoidOurGoalFn_ == nullptr) {
        avoidOurGoal_ = true;
    }
    return this;
}

GoTo *GoTo::avoidTheirGoal(std::function<bool()> condition) {
    avoidTheirGoalFn_ = condition;
    if (avoidTheirGoalFn_ == nullptr) {
        avoidTheirGoal_ = true;
    }
    return this;
}

Status GoTo::execute() {
    if (!goal_) {
        return Status::FAILURE;
    }

    if (!pathPlanner_) {
        return Status::FAILURE;
    }

    Vec2 goal = goal_();
    Vec2 origin = world().playerPositionVec2(id());

    if ((goal - origin).norm() < 1e-2) {
        return Status::SUCCESS;
    }

    handleObstacles();
    auto obs = obstaclesBuilder_.obstacles();
    auto path = pathPlanner_->findPath(origin, goal, obs);
    obstaclesBuilder_.reset();

    auto pos = world().playerPositionVec2(id());
    auto theta = world().playerOrientation(id()).value();
    Vec2 lookAt;
    Vec2 dir(cosf(theta), sinf(theta));
    lookAt = pos + dir * 1.0f;

    controller().move(path, lookAt);

    return Status::RUNNING;
}

void GoTo::handleObstacles() {
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