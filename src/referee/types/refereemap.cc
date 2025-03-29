#include "refereemap.hh"

void RefereeMap::update(char buffer[], int len) {
    _referee.ParseFromArray(buffer, len);
}

const QString RefereeMap::commandAsString(ssl::Referee_Command command) const {
    switch (command) {
    case 0:
        return "HALT";
    case 1:
        return "STOP";
    case 2:
        return "NORMAL_START";
    case 3:
        return "FORCE_START";
    case 4:
        return "PREPARE_KICKOFF_YELLOW";
    case 5:
        return "PREPARE_KICKOFF_BLUE";
    case 6:
        return "PREPARE_PENALTY_YELLOW";
    case 7:
        return "PREPARE_PENALTY_BLUE";
    case 8:
        return "DIRECT_FREE_YELLOW";
    case 9:
        return "DIRECT_FREE_BLUE";
    case 10:
        return "INDIRECT_FREE_YELLOW (deprecated)";
    case 11:
        return "INDIRECT_FREE_BLUE (deprecated)";
    case 12:
        return "TIMEOUT_YELLOW";
    case 13:
        return "TIMEOUT_BLUE";
    case 14:
        return "GOAL_YELLOW (deprecated)";
    case 15:
        return "GOAL_BLUE (deprecated)";
    case 16:
        return "BALL_PLACEMENT_YELLOW";
    case 17:
        return "BALL_PLACEMENT_BLUE";
    default:
        return "Unknown Command";
    }
}