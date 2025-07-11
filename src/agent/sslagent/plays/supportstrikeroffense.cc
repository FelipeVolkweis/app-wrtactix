#include "agent/sslagent/behaviors/behaviors.hh"
#include "agent/sslagent/compoundbehaviors/compoundbehaviors.hh"

#include "supportstrikeroffense.hh"

PlaySupportStrikerOffense::PlaySupportStrikerOffense(const World &worldRef) : SSLPlay(worldRef) {
    roleDefinitions_ = {
        {0, SSLRoleType::GOALKEEPER,
         [](const PlayerID &player, SSLController &controller, const World &world) {
             return new SupportAttacker(player, controller, world);
         }},
        {1, SSLRoleType::STRIKER,
         [](const PlayerID &player, SSLController &controller, const World &world) {
             return new AttackerSupport(player, controller, world);
         }},
    };
    cacheSuggestedAssignments();
}
