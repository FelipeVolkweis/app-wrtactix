#ifndef SSLROLE_HH
#define SSLROLE_HH

#include "types/types.hh"
#include "world/world.hh"

enum class SSLRoleType { UNKNOWN, GOALKEEPER, STRIKER, BARRIER };

class SSLRole {
public:
    SSLRole(SSLRoleType type) : type_(type) {};
    SSLRole() : type_(SSLRoleType::UNKNOWN) {};

    SSLRoleType getType() const {
        return type_;
    }

    void setType(SSLRoleType type) {
        type_ = type;
    }

    virtual PlayerID getRoleAssignment(const World &world, const QHash<PlayerID, bool> &assignmentTable) = 0;

private:
    SSLRoleType type_;
};

#endif