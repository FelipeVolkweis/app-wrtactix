#ifndef WORLD_HH
#define WORLD_HH

#include "world/worldinfo.hh"

namespace World {
class World;
class WorldInfo;
} // namespace World

namespace World {
class World {
public:
    World();
    ~World();

    
private:
    WorldInfo info_;
};
} // namespace World

#endif