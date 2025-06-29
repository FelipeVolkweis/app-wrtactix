#include "worldinfo.hh"

WorldInfo::WorldInfo() : WorldMap() {}

const Position &WorldInfo::playerPosition(const PlayerID &player) const {
    return GEARSystem::WorldMap::playerPosition(player.teamNum(), player.playerNum());
}

const Angle &WorldInfo::playerOrientation(const PlayerID &player) const {
    return GEARSystem::WorldMap::playerOrientation(player.teamNum(), player.playerNum());
}

const Velocity &WorldInfo::playerVelocity(const PlayerID &player) const {
    return GEARSystem::WorldMap::playerVelocity(player.teamNum(), player.playerNum());
}

const AngularSpeed &WorldInfo::playerAngularSpeed(const PlayerID &player) const {
    return GEARSystem::WorldMap::playerAngularSpeed(player.teamNum(), player.playerNum());
}

bool WorldInfo::ballPossession(const PlayerID &player) const {
    return GEARSystem::WorldMap::ballPossession(player.teamNum(), player.playerNum());
}

const Position WorldInfo::ballPosition() const {
    return this->GEARSystem::WorldMap::ballPosition(0);
}

const Velocity WorldInfo::ballVelocity() const {
    return this->GEARSystem::WorldMap::ballVelocity(0);
}
