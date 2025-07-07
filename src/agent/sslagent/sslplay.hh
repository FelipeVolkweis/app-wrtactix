#ifndef SSLPLAY_HH
#define SSLPLAY_HH

#include "types/types.hh"
#include "world/world.hh"

#include "sslbehavior.hh"
#include "sslcontroller.hh"
#include "sslrole.hh"

class SSLPlay {
public:
    using BehaviorFactory = std::function<SSLBehavior *(const PlayerID &, SSLController &, const World &)>;

    struct RoleDefinition {
        int suggestedPlayerNum;
        SSLRoleType roleType;
        BehaviorFactory factory;
    };

    SSLPlay(const World &worldRef) : world_(worldRef) {}

    SSLBehavior *getBehavior(const SSLRole &role, const PlayerID &player, SSLController &controller);

    const QVector<QPair<int, SSLRoleType>> getSuggestedAssignments() {
        return suggestedAssignments_;
    }

protected:
    void cacheSuggestedAssignments();

    const World &world_;

    QVector<RoleDefinition> roleDefinitions_;
    QVector<QPair<int, SSLRoleType>> suggestedAssignments_;
};

#endif