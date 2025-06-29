#include <QApplication>
#include <QCommandLineParser>

#include "agent/sslagent/sslagent.hh"
#include "constants/config/config.hh"
#include "constants/constants.hh"

int main(int argc, char *argv[]) {
    Config::initialize();
    Const::initialize();

    QApplication app(argc, argv);
    app.setApplicationName("WRTactix");
    app.setApplicationVersion("0.1");

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
    QVector<SSLAgent *> agents;

    for (int i = 0; i < 1; i++) {
        PlayerID id(ourTeamColor, i);
        SSLAgent *agent = new SSLAgent(id, ourFieldSide, *controller);
        agents.push_back(agent);
    }

    while (true) {
        for (SSLAgent *agent : agents) {
            agent->observe();
            if (!agent->isActive()) {
                continue;
            }
            qInfo() << "Agent" << agent->id().toString() << "is active.";
            agent->listen();
            agent->think();
            agent->act();
        }
    }

    int retn = app.exec();
    return retn;
}