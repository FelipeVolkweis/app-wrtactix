#ifndef REFEREE_HH
#define REFEREE_HH

#include <QHostAddress>
#include <QMutex>
#include <QUdpSocket>

#include "referee/types/gamestate.hh"
#include "referee/types/refereemap.hh"
#include "world/world.hh"

class Referee {
public:
    Referee(QHostAddress address, uint16_t port, const World &world);

    bool connect();
    bool bufferize();
    void update();

    const RefereeMap &getRefereeMap() const {
        return *_refereeMap;
    };

    const QString stateAsString() const {
        return _gameState.stateAsString();
    }

    const RefereeStates::State state() const {
        return _gameState.state();
    }

    void setPort(uint16_t port) {
        _port = port;
    }

private:
    RefereeMap *_buffer;
    RefereeMap *_refereeMap;

    GameState _gameState;

    QUdpSocket _socket;
    QHostAddress _address;
    uint16_t _port;

    bool _connected;
    bool _updatePending;

    const int sizeofPacket = 65536;

    QMutex _mutex;
};

#endif // REFEREE_HH