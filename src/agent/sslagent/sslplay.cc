#include "agent/sslagent/behaviors/behaviors.hh"

#include "sslplay.hh"

SSLBehavior *SSLPlay::getBehavior(const SSLRole &role, const PlayerID &player, SSLController &controller) {
    for (const auto &definition : roleDefinitions_) {
        if (definition.roleType == role.getType()) {
            return definition.factory(player, controller, world_);
        }
    }

    return new DoNothing(player, controller, world_);
}

void SSLPlay::cacheSuggestedAssignments() {
    suggestedAssignments_.clear();
    for (const auto &definition : roleDefinitions_) {
        suggestedAssignments_.append(QPair<int, SSLRoleType>(definition.suggestedPlayerNum, definition.roleType));
    }
}