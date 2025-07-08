#include "barrier.hh"

SSLBarrier::SSLBarrier() : SSLRole(SSLRoleType::BARRIER) {}

PlayerID SSLBarrier::getRoleAssignment(const World &world, const QHash<PlayerID, bool> &assignmentTable) {
    PlayerID BarrierID = world.ourGoalieId();

    return BarrierID;
}