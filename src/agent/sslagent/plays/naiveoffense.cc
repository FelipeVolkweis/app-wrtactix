#include "agent/sslagent/behaviors/behaviors.hh"

#include "naiveoffense.hh"

PlayNaiveOffense::PlayNaiveOffense(const World &worldRef) : SSLPlay(worldRef) {
    roleDefinitions_ = {
        {0, SSLRoleType::GOALKEEPER,
         [](const PlayerID &player, SSLController &controller, const World &world) {
             return new GoalKeeper(player, controller, world);
         }},
        {1, SSLRoleType::STRIKER,
         [](const PlayerID &player, SSLController &controller, const World &world) {
             return new ShootToGoal(player, controller, world);
         }},
    };
    cacheSuggestedAssignments();
}