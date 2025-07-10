#include "barriermiddle.hh"

SSLBarrierMiddle::SSLBarrierMiddle() : SSLRole(SSLRoleType::BARRIERMIDDLE) {}

PlayerID SSLBarrierMiddle::getRoleAssignment(const World &world, const QHash<PlayerID, bool> &assignmentTable) {
    float minDistToGoal = MAXFLOAT;
    PlayerID closestToGoal;
    Position ourGoalPosition = (world.ourSide() == Sides::LEFT) ? (world.leftGoal().leftPost()) : 
     (world.rightGoal().leftPost());
    
    // Select a player that has not been assigned yet and is closer to our goal
    for (const auto &playerId : assignmentTable.keys()) {
        if (assignmentTable[playerId]) {
            continue;
        }

        float distance = TwoD::distance(world.playerPosition(playerId), ourGoalPosition);
        if (distance < minDistToGoal) {
            minDistToGoal = distance;
            closestToGoal = playerId;
        }
    }

    return closestToGoal;
}