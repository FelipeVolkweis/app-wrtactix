#include "algorithm/geometry/twod/twod.hh"

#include "goto.hh"

using namespace WRBeT;

GoTo::GoTo(const PlayerID &player, SSLController &controller, const World &world)
    : pathPlanner_(nullptr), SSLAction(player, controller, world, "GoTo") {}

GoTo &GoTo::setGoal(std::function<Vec2()> goal) {
    goal_ = goal;
    return *this;
}

GoTo &GoTo::setPathPlanner(PathPlanner *pathPlanner) {
    pathPlanner_ = pathPlanner;
    return *this;
}

Status GoTo::execute() {
    if (!goal_) {
        return Status::FAILURE;
    }

    if (!pathPlanner_) {
        return Status::FAILURE;
    }

    auto origin = TwoD::positionToVector(world().playerPosition(id()));
    auto goal = goal_();

    if ((goal - origin).norm() < 1e-4) {
        return Status::SUCCESS;
    }

    QVector<Obstacle> obstacles;

    auto path = pathPlanner_->findPath(origin, goal, obstacles);

    if (path.empty()) {
        return Status::FAILURE;
    }

    auto orientation = world().playerOrientation(id()).value();
    Vec2 dir{ cosf(orientation), sinf(orientation) };
    Vec2 lookAt = origin + dir * 5.0f;
    controller().move(path, lookAt);

    return Status::RUNNING;
}
