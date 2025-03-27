#ifndef GOTOLOOKAT_HH
#define GOTOLOOKAT_HH

#include "agent/sslagent/sslaction.hh"
#include "algorithm/pathplanner/pathplanner.hh"

class GoToLookAt : public SSLAction {
public:
    GoToLookAt(const PlayerID &player, SSLController &controller, const World &world);

    GoToLookAt &setGoal(std::function<Vec2()> goal);
    GoToLookAt &setPathPlanner(PathPlanner *pathPlanner);
    GoToLookAt &setLookAt(std::function<Vec2()> lookAt);

    Status execute();

private:
    std::function<Vec2()> goal_;
    std::function<Vec2()> lookAt_;
    PathPlanner *pathPlanner_;
};

#endif