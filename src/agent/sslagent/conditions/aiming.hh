#ifndef AIMING_HH
#define AIMING_HH

#include "world/world.hh"

class Aiming {
public:
    Aiming(const World &world);

    
private:
    const World &world_;
};

#endif