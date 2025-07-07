#ifndef PLAYERID_HH
#define PLAYERID_HH

#include <QHash>
#include <QString>
#include <QtGlobal>

class PlayerID {
public:
    PlayerID(quint8 teamNum, quint8 playerNum) : teamNum_(teamNum), playerNum_(playerNum) {}

    PlayerID() : teamNum_(invalidNum()), playerNum_(invalidNum()) {}

    quint8 teamNum() const {
        return teamNum_;
    };

    quint8 playerNum() const {
        return playerNum_;
    };

    bool isValid() const {
        return teamNum_ != invalidNum() && playerNum_ != invalidNum();
    }

    QString toString() const {
        return QString("PlayerID(%1, %2)").arg(teamNum_).arg(playerNum_);
    }

    bool operator==(const PlayerID &other) const {
        return teamNum_ == other.teamNum_ && playerNum_ == other.playerNum_;
    }

    bool operator!=(const PlayerID &other) const {
        return !(*this == other);
    }

    static quint8 invalidNum() {
        return static_cast<quint8>(-1);
    }

    static PlayerID invalid() {
        return PlayerID(invalidNum(), invalidNum());
    }

private:
    quint8 teamNum_;
    quint8 playerNum_;
};

inline uint qHash(const PlayerID &id, uint seed = 0) {
    return qHash((static_cast<quint16>(id.teamNum()) << 8) | id.playerNum(), seed);
}

#endif