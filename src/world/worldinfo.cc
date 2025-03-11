#include "worldinfo.hh"

namespace World {
WorldInfo::WorldInfo() {}

const Position &WorldInfo::playerPosition(const Types::PlayerID &player) const {
    return GEARSystem::WorldMap::playerPosition(player.teamNum(), player.playerNum());
}

const Angle &WorldInfo::playerOrientation(const Types::PlayerID &player) const {
    return GEARSystem::WorldMap::playerOrientation(player.teamNum(), player.playerNum());
}

const Velocity &WorldInfo::playerVelocity(const Types::PlayerID &player) const {
    return GEARSystem::WorldMap::playerVelocity(player.teamNum(), player.playerNum());
}

const AngularSpeed &WorldInfo::playerAngularSpeed(const Types::PlayerID &player) const {
    return GEARSystem::WorldMap::playerAngularSpeed(player.teamNum(), player.playerNum());
}

bool WorldInfo::ballPossession(const Types::PlayerID &player) const {
    return GEARSystem::WorldMap::ballPossession(player.teamNum(), player.playerNum());
}

const Position WorldInfo::ballPosition() const {
    return GEARSystem::WorldMap::ballPosition(0);
}

const Velocity WorldInfo::ballVelocity() const {
    return GEARSystem::WorldMap::ballVelocity(0);
}

} // namespace World
