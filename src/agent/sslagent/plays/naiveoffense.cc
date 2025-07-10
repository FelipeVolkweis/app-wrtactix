#include "agent/sslagent/behaviors/behaviors.hh"

#include "naiveoffense.hh"

Q_LOGGING_CATEGORY(PLAY_NAIVEOFFENSE, "PLAY_NAIVEOFFENSE")

PlayNaiveOffense::PlayNaiveOffense(const World &worldRef) : SSLPlay(worldRef) {
    roleDefinitions_ = {
        {0, SSLRoleType::GOALKEEPER,
         [](const PlayerID &player, SSLController &controller, const World &world) {
             return new GoalKeeper(player, controller, world);
        }},
        {0, SSLRoleType::BARRIERRIGHT,
         [](const PlayerID &player, SSLController &controller, const World &world) {
            qCCritical(PLAY_NAIVEOFFENSE) << "RIGHT playerNum: " << player.playerNum();
            return new BarrierRight(player, controller, world);
        }},
        {2, SSLRoleType::BARRIERLEFT,
         [](const PlayerID &player, SSLController &controller, const World &world) {
            qCCritical(PLAY_NAIVEOFFENSE) << "LEFT playerNum: " << player.playerNum();
            return new BarrierLeft(player, controller, world);
        }},
        // {2, SSLRoleType::BARRIERMIDDLE,
        //  [](const PlayerID &player, SSLController &controller, const World &world) {
        //     qCCritical(PLAY_NAIVEOFFENSE) << "LEFT playerNum: " << player.playerNum();
        //     return new BarrierMiddle(player, controller, world);
        // }},
        {3, SSLRoleType::STRIKER,
        [](const PlayerID &player, SSLController &controller, const World &world) {
            return new ShootToGoal(player, controller, world);
            // return new CalibrateLinear(player, controller, world);
            // return new CalibrateAngular(player, controller, world);
        }},
    };

    for(RoleDefinition roleDefinition : roleDefinitions_) {
        qCCritical(PLAY_NAIVEOFFENSE) << "RoleDefiniton: " << roleDefinition.suggestedPlayerNum;
    }
    cacheSuggestedAssignments();
}