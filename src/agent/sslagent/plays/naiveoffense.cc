#include "naiveoffense.hh"

using namespace WRBeT;

Node *PlayNaiveOffense::getBehavior(const SSLRole &role, const PlayerID &player, SSLController &controller) {
    switch (role.getType()) {
    case SSLRoleType::GOALKEEPER:
    case SSLRoleType::STRIKER:
    default:
        break;
    }
    return nullptr;
}