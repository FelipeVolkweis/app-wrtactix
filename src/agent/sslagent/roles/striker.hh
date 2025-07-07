#ifndef STRIKER_HH
#define STRIKER_HH

#include "agent/sslagent/sslrole.hh"

class SSLStriker : public SSLRole {
public:
    SSLStriker();

    PlayerID getRoleAssignment(const World &world, const QHash<PlayerID, bool> &assignmentTable);
};

#endif