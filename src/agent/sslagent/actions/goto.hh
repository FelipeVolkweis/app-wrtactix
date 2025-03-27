#ifndef GOTO_HH
#define GOTO_HH

#include "agent/sslagent/sslaction.hh"
#include "algorithm/pathplanner/pathplanner.hh"

class GoTo : public SSLAction {
public:
    GoTo(const PlayerID &player, SSLController &controller, const World &world);

    GoTo &setGoal(std::function<Vec2()> goal);
    GoTo &setPathPlanner(PathPlanner *pathPlanner);

    Status execute();

private:
    std::function<Vec2()> goal_;
    PathPlanner *pathPlanner_;
};

#endif