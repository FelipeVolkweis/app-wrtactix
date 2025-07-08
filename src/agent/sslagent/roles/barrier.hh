#ifndef SSLBARRIER_HH
#define SSLBARRIER_HH

#include "agent/sslagent/sslrole.hh"

class SSLBarrier : public SSLRole {
public:
    SSLBarrier();
    PlayerID getRoleAssignment(const World &world, const QHash<PlayerID, bool> &assignmentTable);
};

#endif