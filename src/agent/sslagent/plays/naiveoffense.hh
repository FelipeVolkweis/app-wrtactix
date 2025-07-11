#ifndef NAIVEOFFENSE_HH
#define NAIVEOFFENSE_HH

#include "agent/sslagent/sslplay.hh"

class PlayNaiveOffense : public SSLPlay {
public:
    PlayNaiveOffense(const World &worldRef);
};

#endif