#ifndef PLAYERID_HH
#define PLAYERID_HH

#include <QString>
#include <QtGlobal>

class PlayerID {
public:
    PlayerID(quint8 teamNum, quint8 playerNum) : teamNum_(teamNum), playerNum_(playerNum) {}

    quint8 teamNum() const {
        return teamNum_;
    };

    quint8 playerNum() const {
        return playerNum_;
    };

    QString toString() const {
        return QString("PlayerID(%1, %2)").arg(teamNum_).arg(playerNum_);
    }

    bool operator==(const PlayerID &other) const {
        return teamNum_ == other.teamNum_ && playerNum_ == other.playerNum_;
    }
    
    bool operator!=(const PlayerID &other) const {
        return !(*this == other);
    }

private:
    quint8 teamNum_;
    quint8 playerNum_;
};

#endif