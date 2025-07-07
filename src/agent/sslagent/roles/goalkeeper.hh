#ifndef GOALKEEPER_HH
#define GOALKEEPER_HH

#include "agent/sslagent/sslrole.hh"

class SSLGoalkeeper : public SSLRole {
public:
    SSLGoalkeeper();
    PlayerID getRoleAssignment(const World &world, const QHash<PlayerID, bool> &assignmentTable);
};

#endif // GOALKEEPER_HH