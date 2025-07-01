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

    GoToLookAt *avoidTeammates(std::function<bool()> condition = nullptr);
    GoToLookAt *avoidOpponents(std::function<bool()> condition = nullptr);
    GoToLookAt *avoidBall(std::function<bool()> condition = nullptr);
    GoToLookAt *avoidOurGoal(std::function<bool()> condition = nullptr);
    GoToLookAt *avoidTheirGoal(std::function<bool()> condition = nullptr);

    Status execute();

private:
    std::function<Vec2()> goal_;
    std::function<Vec2()> lookAt_;
    PathPlanner *pathPlanner_;
    ObstaclesBuilder obstaclesBuilder_;

    std::function<bool()> avoidTeammates_;
    std::function<bool()> avoidOpponents_;
    std::function<bool()> avoidBall_;
    std::function<bool()> avoidOurGoal_;
    std::function<bool()> avoidTheirGoal_;

    void handleObstacles();
};

#endif