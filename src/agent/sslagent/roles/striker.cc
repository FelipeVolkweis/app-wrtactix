#include "algorithm/geometry/twod/twod.hh"

#include "striker.hh"
#include <limits>

SSLStriker::SSLStriker() : SSLRole(SSLRoleType::STRIKER) {}

PlayerID SSLStriker::getRoleAssignment(const World &world, const QHash<PlayerID, bool> &assignmentTable) {
    float distToBall = MAXFLOAT;
    PlayerID closestToBall;
    for (const auto &playerId : assignmentTable.keys()) {
        if (assignmentTable[playerId]) {
            continue;
        }

        float distance = TwoD::distance(world.playerPositionVec2(playerId), world.ballPositionVec2());
        if (distance < distToBall) {
            distToBall = distance;
            closestToBall = playerId;
        }
    }
    return closestToBall;
}