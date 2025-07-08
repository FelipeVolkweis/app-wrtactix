#include "agent/sslagent/behaviors/behaviors.hh"

#include "naiveoffense.hh"

PlayNaiveOffense::PlayNaiveOffense(const World &worldRef) : SSLPlay(worldRef) {
    roleDefinitions_ = {
        // {0, SSLRoleType::GOALKEEPER,
        //  [](const PlayerID &player, SSLController &controller, const World &world) {
        //      return new GoalKeeper(player, controller, world);
        //  }},
        {0, SSLRoleType::BARRIER,
        [](const PlayerID &player, SSLController &controller, const World &world) {
            return new Barrier(player, controller, world);
        }},
        {1, SSLRoleType::STRIKER,
         [](const PlayerID &player, SSLController &controller, const World &world) {
            return new ShootToGoal(player, controller, world);
            // return new CalibrateLinear(player, controller, world);
            // return new CalibrateAngular(player, controller, world);
         }},
    };
    cacheSuggestedAssignments();
}