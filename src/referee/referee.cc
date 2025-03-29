#include <QLoggingCategory>
#include <QMutexLocker>

#include "referee.hh"

Q_LOGGING_CATEGORY(REFEREE, "Referee")

Referee::Referee(QHostAddress address, uint16_t port, const World &world)
    : _address(address), _port(port), _connected(false), _updatePending(false), _gameState(world) {
    _buffer = new RefereeMap();
    _refereeMap = new RefereeMap();
}

bool Referee::connect() {
    if (!_socket.bind(QHostAddress::AnyIPv4, _port, QUdpSocket::ShareAddress)) {
        qCWarning(REFEREE) << "Failed to bind to port" << _port;
        return false;
    }
    if (!_socket.joinMulticastGroup(_address)) {
        qCWarning(REFEREE) << "Failed to join multicast group" << _address;
        return false;
    }

    qInfo(REFEREE) << "Connected to referee server at" << _address << ":" << _port;
    _connected = true;
    return true;
}

bool Referee::bufferize() {
    if (!_connected) {
        qCWarning(REFEREE) << "Not connected to referee server";
        return false;
    }

    int len = 0;
    char buffer[sizeofPacket];

    while (_socket.hasPendingDatagrams()) {
        len = _socket.readDatagram(buffer, sizeofPacket);
        if (len == -1) {
            qCWarning(REFEREE) << "Failed to read datagram";
            return false;
        }

        _buffer->update(buffer, len);
        _updatePending = true;
    }

    return true;
}

void Referee::update() {
    if (!_updatePending) {
        return;
    }

    if (_buffer->command() != _refereeMap->command()) {
        qInfo(REFEREE) << "Referee said" << _buffer->commandAsString();
    }

    _updatePending = false;

    _mutex.lock();
    qSwap(_buffer, _refereeMap);
    _mutex.unlock();
    _gameState.stateMachine(_refereeMap->commandAsString());
}