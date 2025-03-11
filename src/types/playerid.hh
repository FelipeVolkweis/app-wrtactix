#ifndef PLAYERID_HH
#define PLAYERID_HH

#include <QtGlobal>

namespace Types {
class PlayerID {
public:
    PlayerID(quint8 teamNum, quint8 playerNum) : teamNum_(teamNum), playerNum_(playerNum) {}

    quint8 teamNum() const {
        return teamNum_;
    };

    quint8 playerNum() const {
        return playerNum_;
    };

private:
    quint8 teamNum_;
    quint8 playerNum_;
};
} // namespace Types

#endif