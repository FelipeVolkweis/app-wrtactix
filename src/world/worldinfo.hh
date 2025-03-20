#ifndef WORLDINFO_HH
#define WORLDINFO_HH

#include "types/playerid.hh"

#include <GEARSystem/worldmap.hh>

class WorldInfo : public GEARSystem::WorldMap {
public:
    WorldInfo();

    const Position &playerPosition(const PlayerID &player) const;
    const Angle &playerOrientation(const PlayerID &player) const;
    const Velocity &playerVelocity(const PlayerID &player) const;
    const AngularSpeed &playerAngularSpeed(const PlayerID &player) const;
    bool ballPossession(const PlayerID &player) const;

    const Position ballPosition() const;
    const Velocity ballVelocity() const;
};

#endif