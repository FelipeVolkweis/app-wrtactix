#ifndef GOTOLOOKAT_HH
#define GOTOLOOKAT_HH

#include "agent/sslagent/sslaction.hh"
#include "algorithm/pathplanner/pathplanner.hh"
#include "agent/wrappers/obstaclesbuilder.hh"

class GoToLookAt : public SSLAction {
public:
    GoToLookAt(const PlayerID &player, SSLController &controller, const World &world);

    GoToLookAt *setGoal(std::function<Vec2()> goal);
    GoToLookAt *setPathPlanner(PathPlanner *pathPlanner);
    GoToLookAt *setLookAt(std::function<Vec2()> lookAt);

    GoToLookAt *avoidTeammates();
    GoToLookAt *avoidOpponents();
    GoToLookAt *avoidBall();
    GoToLookAt *avoidOurGoal();
    GoToLookAt *avoidTheirGoal();

    Status execute();

private:
    std::function<Vec2()> goal_;
    std::function<Vec2()> lookAt_;
    PathPlanner *pathPlanner_;
    ObstaclesBuilder obstaclesBuilder_;

    bool avoidTeammates_ = false;
    bool avoidOpponents_ = false;
    bool avoidBall_ = false;
    bool avoidOurGoal_ = false;
    bool avoidTheirGoal_ = false;

    void handleObstacles();
};

#endif