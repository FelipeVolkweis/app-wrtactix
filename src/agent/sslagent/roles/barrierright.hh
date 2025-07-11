#ifndef SSLBARRIERRIGHT_HH
#define SSLBARRIERRIGHT_HH

#include "agent/sslagent/sslrole.hh"

class SSLBarrierRight : public SSLRole {
public:
    SSLBarrierRight();
    PlayerID getRoleAssignment(const World &world, const QHash<PlayerID, bool> &assignmentTable);
};

#endif