#ifndef WORLDINFO_HH
#define WORLDINFO_HH

#include "types/playerid.hh"

#include <GEARSystem/worldmap.hh>

namespace World {
class WorldInfo : public GEARSystem::WorldMap {
public:
    WorldInfo();

    const Position &playerPosition(const Types::PlayerID &player) const;
    const Angle &playerOrientation(const Types::PlayerID &player) const;
    const Velocity &playerVelocity(const Types::PlayerID &player) const;
    const AngularSpeed &playerAngularSpeed(const Types::PlayerID &player) const;
    bool ballPossession(const Types::PlayerID &player) const;

    const Position ballPosition() const;
    const Velocity ballVelocity() const;
};
} // namespace World

#endif