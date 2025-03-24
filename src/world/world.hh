#ifndef WORLD_HH
#define WORLD_HH

#include "types/types.hh"
#include "world/worldinfo.hh"

#include <GEARSystem/controller.hh>

class World {
public:
    World(GEARSystem::Controller &controller);
    ~World();

    bool connectToBackbone();

    void update();

    void setSide(Sides::Side side);
    void setColor(Colors::Color color);

    const Colors::Color ourColor() const {
        return color_ == Colors::BLUE ? Colors::BLUE : Colors::YELLOW;
    }

    const Colors::Color theirColor() const {
        return color_ == Colors::BLUE ? Colors::YELLOW : Colors::BLUE;
    }

    const Sides::Side ourSide() const {
        return side_ == Sides::RIGHT ? Sides::RIGHT : Sides::LEFT;
    }

    const Sides::Side theirSide() const {
        return side_ == Sides::RIGHT ? Sides::LEFT : Sides::RIGHT;
    }

    const Position &playerPosition(const PlayerID &player) const {
        return info_->playerPosition(player);
    }

    const Angle &playerOrientation(const PlayerID &player) const {
        return info_->playerOrientation(player);
    }

    const Velocity &playerVelocity(const PlayerID &player) const {
        return info_->playerVelocity(player);
    }

    const AngularSpeed &playerAngularSpeed(const PlayerID &player) const {
        return info_->playerAngularSpeed(player);
    }

    bool ballPossession(const PlayerID &player) const {
        return info_->ballPossession(player);
    }

    const Position ballPosition() const {
        return info_->ballPosition();
    }

    const Velocity ballVelocity() const {
        return info_->ballVelocity();
    }

    const Position &fieldTopRightCorner() const {
        return info_->fieldTopRightCorner();
    }

    const Position &fieldTopLeftCorner() const {
        return info_->fieldTopLeftCorner();
    }

    const Position &fieldBottomLeftCorner() const {
        return info_->fieldBottomLeftCorner();
    }

    const Position &fieldBottomRightCorner() const {
        return info_->fieldBottomRightCorner();
    }

    const Position &fieldCenter() const {
        return info_->fieldCenter();
    }

    const Goal &leftGoal() const {
        return info_->leftGoal();
    }

    const Goal &rightGoal() const {
        return info_->rightGoal();
    }

    const Position &leftPenaltyMark() const {
        return info_->leftPenaltyMark();
    }

    const Position &rightPenaltyMark() const {
        return info_->rightPenaltyMark();
    }

    float fieldCenterRadius() const {
        return info_->fieldCenterRadius();
    }

private:
    void update(WorldInfo &info);
    void bufferizeBall(WorldInfo &info, uint8 ballNum);
    void bufferizeBallPossession(WorldInfo &info, uint8 teamNum, uint8 playerNum);
    void bufferizeTeam(WorldInfo &info, uint8 teamNum);
    void bufferizePlayer(WorldInfo &info, uint8 teamNum, uint8 playerNum);
    void bufferizeFieldGeometry(WorldInfo &info);

    WorldInfo *info_;
    WorldInfo *infoBuffer_;

    GEARSystem::Controller &controller_;

    QMutex mutex_;

    Sides::Side side_;
    Colors::Color color_;
};

#endif