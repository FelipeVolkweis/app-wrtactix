#ifndef GOTOLOOKAT_HH
#define GOTOLOOKAT_HH

#include "agent/sslagent/sslaction.hh"
#include "agent/wrappers/obstaclesbuilder.hh"
#include "algorithm/pathplanner/pathplanner.hh"

#include <functional>

class GoToLookAt : public SSLAction {
public:
    GoToLookAt(const PlayerID &player, SSLController &controller, const World &world);

    GoToLookAt *setGoal(std::function<Vec2()> goal);
    GoToLookAt *setPathPlanner(PathPlanner *pathPlanner);
    GoToLookAt *setLookAt(std::function<Vec2()> lookAt);
    GoToLookAt *setWaypoint(std::function<Vec2()> waypoint);
    GoToLookAt *setSpeedBoost(std::function<float()> speedBoost);

    GoToLookAt *avoidTeammates(std::function<bool()> condition = nullptr);
    GoToLookAt *avoidOpponents(std::function<bool()> condition = nullptr);
    GoToLookAt *avoidBall(std::function<bool()> condition = nullptr);
    GoToLookAt *avoidOurGoal(std::function<bool()> condition = nullptr);
    GoToLookAt *avoidTheirGoal(std::function<bool()> condition = nullptr);

    Status execute();

private:
    std::function<Vec2()> goal_;
    std::function<Vec2()> lookAt_;
    std::function<Vec2()> waypoint_;
    std::function<float()> speedBoost_;
    PathPlanner *pathPlanner_;
    ObstaclesBuilder obstaclesBuilder_;

    std::function<bool()> avoidTeammatesFn_;
    std::function<bool()> avoidOpponentsFn_;
    std::function<bool()> avoidBallFn_;
    std::function<bool()> avoidOurGoalFn_;
    std::function<bool()> avoidTheirGoalFn_;

    bool avoidTeammates_ = false;
    bool avoidOpponents_ = false;
    bool avoidBall_ = false;
    bool avoidOurGoal_ = false;
    bool avoidTheirGoal_ = false;

    void handleObstacles();
};

#endif