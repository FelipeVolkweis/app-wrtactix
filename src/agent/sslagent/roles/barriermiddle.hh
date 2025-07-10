#ifndef SSLBARRIERMIDDLE_HH
#define SSLBARRIERMIDDLE_HH

#include "agent/sslagent/sslrole.hh"

class SSLBarrierMiddle : public SSLRole {
public:
    SSLBarrierMiddle();
    PlayerID getRoleAssignment(const World &world, const QHash<PlayerID, bool> &assignmentTable);
};

#endif