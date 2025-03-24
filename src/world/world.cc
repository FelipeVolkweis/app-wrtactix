#include "algorithm/geometry/twod/twod.hh"
#include "types/playerid.hh"

#include "world.hh"

#define BALLS 5
#define TEAMS 2
#define PLAYERS 16

World::World(GEARSystem::Controller &controller) : controller_(controller) {
    info_ = new WorldInfo();
    infoBuffer_ = new WorldInfo();
    side_ = Sides::UNDEFINED;
    color_ = Colors::NONE;

    for (int i = 0; i < BALLS; i++) {
        info_->addBall(i);
        infoBuffer_->addBall(i);
    }

    for (int i = 0; i < TEAMS; i++) {
        info_->addTeam(i, "Team " + QString::number(i));
        infoBuffer_->addTeam(i, "Team " + QString::number(i));

        for (int j = 0; j < PLAYERS; j++) {
            info_->addPlayer(i, j);
            infoBuffer_->addPlayer(i, j);
        }
    }
}

World::~World() {
    delete info_;
    delete infoBuffer_;
}

bool World::connectToBackbone() {
    return controller_.connect("localhost", 0);
}

void World::update() {
    if (!controller_.isConnected()) {
        connectToBackbone();

        return;
    }

    update(*infoBuffer_);

    mutex_.lock();
    std::swap(info_, infoBuffer_);
    mutex_.unlock();
}

void World::update(WorldInfo &info) {
    bufferizeFieldGeometry(info);

    for (int i = 0; i < BALLS; i++) {
        bufferizeBall(info, i);
    }

    for (int i = 0; i < TEAMS; i++) {
        bufferizeTeam(info, i);
    }
}

void World::setSide(Sides::Side side) {
    side_ = side;
}

void World::setColor(Colors::Color color) {
    color_ = color;
}

void World::bufferizeBall(WorldInfo &info, uint8 ballNum) {
    info.setBallPosition(ballNum, controller_.ballPosition(ballNum));
    info.setBallVelocity(ballNum, controller_.ballVelocity(ballNum));
}

// TODO:
/*
    Teoricamente a posse de bola vem do sensor, porém o eye nunca setta este valor.
    Então é necessário que o coach sette este valor.
*/
void World::bufferizeBallPossession(WorldInfo &info, uint8 teamNum, uint8 playerNum) {
    PlayerID playerId(teamNum, playerNum);
    float dist = TwoD::distance(info.playerPosition(playerId), info.ballPosition());
    bool hasPossession = TwoD::distance(info.playerPosition(playerId), info.ballPosition()) < 0.15;

    info.setBallPossession(teamNum, playerNum, hasPossession);
}

void World::bufferizeTeam(WorldInfo &info, uint8 teamNum) {
    QList<uint8> players = info.players(teamNum);
    for (auto player : players) {
        bufferizePlayer(info, teamNum, player);
        if (controller_.playerPosition(teamNum, player).isValid() &&
            !controller_.playerPosition(teamNum, player).isUnknown()) {
            bufferizeBallPossession(info, teamNum, player);
        }
    }
}

void World::bufferizePlayer(WorldInfo &info, uint8 teamNum, uint8 playerNum) {
    info.setPlayerPosition(teamNum, playerNum, controller_.playerPosition(teamNum, playerNum));
    info.setPlayerOrientation(teamNum, playerNum, controller_.playerOrientation(teamNum, playerNum));
    info.setPlayerVelocity(teamNum, playerNum, controller_.playerVelocity(teamNum, playerNum));
    info.setPlayerAngularSpeed(teamNum, playerNum, controller_.playerAngularSpeed(teamNum, playerNum));
}

void World::bufferizeFieldGeometry(WorldInfo &info) {
    info.setFieldTopRightCorner(controller_.fieldTopRightCorner());
    info.setFieldTopLeftCorner(controller_.fieldTopLeftCorner());
    info.setFieldBottomLeftCorner(controller_.fieldBottomLeftCorner());
    info.setFieldBottomRightCorner(controller_.fieldBottomRightCorner());
    info.setFieldCenter(controller_.fieldCenter());

    info.setLeftGoalPosts(controller_.leftGoal().leftPost(), controller_.leftGoal().rightPost());
    info.setRightGoalPosts(controller_.rightGoal().leftPost(), controller_.rightGoal().rightPost());

    info.setGoalArea(controller_.rightGoal().getAreaLength(), controller_.rightGoal().getAreaWidth(),
                     controller_.rightGoal().getAreaRoundedRadius());
    info.setGoalDepth(controller_.rightGoal().getDepth());

    info.setLeftPenaltyMark(controller_.leftPenaltyMark());
    info.setRightPenaltyMark(controller_.rightPenaltyMark());

    info.setFieldCenterRadius(controller_.fieldCenterRadius());
}