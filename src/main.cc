#include <QApplication>
#include <QCommandLineParser>
#include <QLoggingCategory>

#include "agent/sslagent/sslagent.hh"
#include "coach/sslcoach/sslcoach.hh"
#include "constants/config/config.hh"
#include "constants/constants.hh"
#include "utils/logger/logger.hh"

Q_LOGGING_CATEGORY(MAIN, "Main")

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("WRTactix");
    app.setApplicationVersion("0.1");

    Config::initialize();
    Const::initialize();
    Logger::init();

    QCommandLineParser parser;
    parser.setApplicationDescription("WRCoach application help.");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption teamColorConfig({"t", "team"}, "Sets the team color ('yellow' or 'blue', default='blue').",
                                       "team", "blue");
    QCommandLineOption fieldSideConfig({"s", "side"}, "Sets the field side ('right' or 'left', default='left').",
                                       "side", "left");
    parser.addOption(teamColorConfig);
    parser.addOption(fieldSideConfig);
    parser.process(app);

    Colors::Color ourTeamColor = Colors::BLUE;
    Sides::Side ourFieldSide = Sides::LEFT;

    if (parser.isSet(teamColorConfig)) {
        QString teamColor = parser.value(teamColorConfig);
        if (teamColor == "yellow") {
            ourTeamColor = Colors::YELLOW;
        } else if (teamColor == "blue") {
            ourTeamColor = Colors::BLUE;
        } else {
            qWarning() << "Invalid team color. Defaulting to blue.";
        }
    }
    if (parser.isSet(fieldSideConfig)) {
        QString fieldSide = parser.value(fieldSideConfig);
        if (fieldSide == "left") {
            ourFieldSide = Sides::LEFT;
        } else if (fieldSide == "right") {
            ourFieldSide = Sides::RIGHT;
        } else {
            qWarning() << "Invalid field side. Defaulting to left.";
        }
    }

    GEARSystem::Controller *controller = new GEARSystem::Controller();
    World world(*controller);
    world.setColor(ourTeamColor);
    world.setSide(ourFieldSide);

    Referee referee(QHostAddress(Const::Referee::ip_address), Const::Referee::port, world);
    referee.connect();

    QVector<SSLAgent *> agents;

    for (int i = 0; i < 16; i++) {
        PlayerID id(ourTeamColor, i);
        SSLAgent *agent = new SSLAgent(id, ourFieldSide, *controller, world, referee);
        agents.push_back(agent);
    }

    SSLCoach coach(world, referee, agents);

    while (true) {
        QElapsedTimer timer;
        timer.start();
        
        world.update();
        referee.bufferize();
        referee.update();
        coach.delegatePlaysAndRoles();

        for (SSLAgent *agent : agents) {
            if (!agent->isActive()) {
                continue;
            }
            agent->think();
            agent->act();
        }
        if (timer.elapsed() > 10) {
            qCInfo(MAIN) << timer.elapsed() << "ms in total";
        }
    }

    int retn = app.exec();
    return retn;
}