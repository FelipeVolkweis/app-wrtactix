#ifndef GOTO_HH
#define GOTO_HH

#include "agent/sslagent/sslaction.hh"
#include "algorithm/pathplanner/pathplanner.hh"
#include "agent/wrappers/obstaclesbuilder.hh"

#include <functional>

class GoTo : public SSLAction {
public:
    GoTo(const PlayerID &player, SSLController &controller, const World &world);

    GoTo *setGoal(std::function<Vec2()> goal);
    GoTo *setPathPlanner(PathPlanner *pathPlanner);

    GoTo *avoidTeammates(std::function<bool()> condition = nullptr);
    GoTo *avoidOpponents(std::function<bool()> condition = nullptr);
    GoTo *avoidBall(std::function<bool()> condition = nullptr);
    GoTo *avoidOurGoal(std::function<bool()> condition = nullptr);
    GoTo *avoidTheirGoal(std::function<bool()> condition = nullptr);

    Status execute();

private:
    std::function<Vec2()> goal_;
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