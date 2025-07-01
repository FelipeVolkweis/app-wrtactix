#include "obstaclesbuilder.hh"

#include "constants/constants.hh"

void ObstaclesBuilder::avoidTeammates() {
    for (const auto &pid : world_.ourAvailablePlayers()) {
        if (pid != id_) {
            const auto &pos = world_.playerPositionVec2(pid);
            float radius = 2 * Const::Physics::robot_radius + 0.1f;
            obstacles_.push_back(Obstacle(pos, radius));
        }
    }
}

void ObstaclesBuilder::avoidOpponents() {
    for (const auto &pid : world_.theirAvailablePlayers()) {
        const auto &pos = world_.playerPositionVec2(pid);
        float radius = 2 * Const::Physics::robot_radius + 0.1f;
        obstacles_.push_back(Obstacle(pos, radius));
    }
}

void ObstaclesBuilder::avoidBall() {
    const auto &ballPos = world_.ballPositionVec2();
    float radius = Const::Physics::ball_radius + 0.1f;
    obstacles_.push_back(Obstacle(ballPos, radius));
}

void ObstaclesBuilder::avoidOurGoal() {
    auto bottomLeft = world_.ourSide() == Sides::LEFT ? bottomLeftLeftGoal() : bottomLeftRightGoal();
    auto topRight = world_.ourSide() == Sides::LEFT ? topRightLeftGoal() : topRightRightGoal();
    obstacles_.push_back(Obstacle(bottomLeft, topRight));
}

void ObstaclesBuilder::avoidTheirGoal() {
    auto bottomLeft = world_.ourSide() == Sides::LEFT ? bottomLeftRightGoal() : bottomLeftLeftGoal();
    auto topRight = world_.ourSide() == Sides::LEFT ? topRightRightGoal() : topRightLeftGoal();
    obstacles_.push_back(Obstacle(bottomLeft, topRight));
}

void ObstaclesBuilder::reset() {
    obstacles_.clear();
}

Vec2 ObstaclesBuilder::bottomLeftLeftGoal() const {
    float x = world_.fieldBottomLeftCorner().x();
    float y = -world_.leftGoal().getAreaLength() / 2.0f;

    return Vec2(x, y);
}

Vec2 ObstaclesBuilder::topRightLeftGoal() const {
    float x = world_.fieldBottomLeftCorner().x() + world_.leftGoal().getAreaWidth();
    float y = world_.leftGoal().getAreaLength() / 2.0f;

    return Vec2(x, y);
}

Vec2 ObstaclesBuilder::bottomLeftRightGoal() const {
    float x = world_.fieldBottomRightCorner().x() - world_.rightGoal().getAreaWidth();
    float y = -world_.rightGoal().getAreaLength() / 2.0f;

    return Vec2(x, y);
}

Vec2 ObstaclesBuilder::topRightRightGoal() const {
    float x = world_.fieldBottomRightCorner().x();
    float y = world_.rightGoal().getAreaLength() / 2.0f;

    return Vec2(x, y);
}