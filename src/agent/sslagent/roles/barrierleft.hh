#ifndef SSLBARRIERLEFT_HH
#define SSLBARRIERLEFT_HH

#include "agent/sslagent/sslrole.hh"

class SSLBarrierLeft : public SSLRole {
public:
    SSLBarrierLeft();
    PlayerID getRoleAssignment(const World &world, const QHash<PlayerID, bool> &assignmentTable);
};

#endif