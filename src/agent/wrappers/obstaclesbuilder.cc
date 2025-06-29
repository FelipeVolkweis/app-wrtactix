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
}

void ObstaclesBuilder::avoidTheirGoal() {
}

void ObstaclesBuilder::reset() {
    obstacles_.clear();
}