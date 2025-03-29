#ifndef REFEREEMAP_HH
#define REFEREEMAP_HH

#include <QString>
#include <QtGlobal>

#include "types/types.hh"

#ifndef Q_MOC_RUN
#include <3rdparty/proto/pb/ssl_gc_referee_message.pb.h>
#endif

class RefereeMap {
public:
    void update(char buffer[], int len);

    const ssl::Referee_Stage stage() const {
        return _referee.stage();
    }

    const uint64_t packetTimestamp() const {
        return _referee.packet_timestamp();
    }

    const int64_t stageTimeLeft() const {
        return _referee.stage_time_left();
    }

    const ssl::Referee_Command command() const {
        return _referee.command();
    }

    const uint64_t commandCounter() const {
        return _referee.command_timestamp();
    }

    const ssl::Referee_TeamInfo blueInfo() const {
        return _referee.blue();
    }

    const ssl::Referee_TeamInfo yellowInfo() const {
        return _referee.yellow();
    }

    // Elaborar novos getters para TeamInfo conforme for necessário
    const ssl::Referee_TeamInfo teamInfo(Colors::Color color) const {
        if (color == Colors::BLUE) {
            return blueInfo();
        } else {
            return yellowInfo();
        }
    }

    const quint8 teamInfoGoalie(Colors::Color color) const {
        return teamInfo(color).goalkeeper();
    }

    const QString commandAsString(ssl::Referee_Command command) const;

    const QString commandAsString() const {
        return commandAsString(command());
    }

private:
    ssl::Referee _referee;
    ssl::Referee_Command _lastCommand;
    bool _ballKicked;
};

#endif // REFEREEMAP_HH