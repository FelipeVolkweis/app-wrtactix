#include "goalkeeper.hh"

SSLGoalkeeper::SSLGoalkeeper() : SSLRole(SSLRoleType::GOALKEEPER) {}

PlayerID SSLGoalkeeper::getRoleAssignment(const World &world, const QHash<PlayerID, bool> &assignmentTable) {
    PlayerID goalkeeperID = world.ourGoalieId();

    return goalkeeperID;
}