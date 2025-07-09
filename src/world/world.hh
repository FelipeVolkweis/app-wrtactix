#ifndef WORLD_HH
#define WORLD_HH

#include "algorithm/geometry/twod/twod.hh"
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

    Vec2 playerPositionVec2(const PlayerID &player) const {
        return TwoD::positionToVector(info_->playerPosition(player));
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

    bool playerIsActive(const PlayerID &player) const {
        return info_->playerPosition(player).isValid() && !info_->playerPosition(player).isUnknown();
    }

    bool ballPossession(const PlayerID &player) const {
        return info_->ballPossession(player);
    }

    QList<PlayerID> availablePlayers(Colors::Color color) const;
    QList<PlayerID> ourAvailablePlayers() const;
    QList<PlayerID> theirAvailablePlayers() const;
    QList<PlayerID> ourPlayers() const;

    const Position ballPosition() const {
        return info_->ballPosition();
    }

    Vec2 ballPositionVec2() const {
        return TwoD::positionToVector(info_->ballPosition());
    }

    const Velocity ballVelocity() const {
        return info_->ballVelocity();
    }

    const Vec2 ballVelocityVec2() const { 
        return TwoD::velocityToVector(info_->ballVelocity());
    }

    PlayerID closestPlayerToBall(Colors::Color color) const;
    PlayerID closestPlayerToBall() const;

    PlayerID ourClosestPlayerToBall() const {
        return closestPlayerToBall(ourColor());
    }

    PlayerID theirClosestPlayerToBall() const {
        return closestPlayerToBall(theirColor());
    }

    const Position &fieldTopRightCorner() const {
        return info_->fieldTopRightCorner();
    }

    Vec2 fieldTopRightCornerVec2() const {
        return TwoD::positionToVector(info_->fieldTopRightCorner());
    }

    const Position &fieldTopLeftCorner() const {
        return info_->fieldTopLeftCorner();
    }

    Vec2 fieldTopLeftCornerVec2() const {
        return TwoD::positionToVector(info_->fieldTopLeftCorner());
    }

    const Position &fieldBottomLeftCorner() const {
        return info_->fieldBottomLeftCorner();
    }

    Vec2 fieldBottomLeftCornerVec2() const {
        return TwoD::positionToVector(info_->fieldBottomLeftCorner());
    }

    const Position &fieldBottomRightCorner() const {
        return info_->fieldBottomRightCorner();
    }

    Vec2 fieldBottomRightCornerVec2() const {
        return TwoD::positionToVector(info_->fieldBottomRightCorner());
    }

    const Position &fieldCenter() const {
        return info_->fieldCenter();
    }

    Vec2 fieldCenterVec2() const {
        return TwoD::positionToVector(info_->fieldCenter());
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

    Vec2 leftPenaltyMarkVec2() const {
        return TwoD::positionToVector(info_->leftPenaltyMark());
    }

    const Position &rightPenaltyMark() const {
        return info_->rightPenaltyMark();
    }

    Vec2 rightPenaltyMarkVec2() const {
        return TwoD::positionToVector(info_->rightPenaltyMark());
    }

    float fieldCenterRadius() const {
        return info_->fieldCenterRadius();
    }

    void setOurGoalieId(PlayerID goalie) {
        QMutexLocker locker(&refereeMutex_);
        ourGoalieId_ = goalie;
    }

    void setTheirGoalieId(PlayerID goalie) {
        QMutexLocker locker(&refereeMutex_);
        theirGoalieId_ = goalie;
    }

    const PlayerID &ourGoalieId() const {
        return ourGoalieId_;
    }

    const PlayerID &theirGoalieId() const {
        return theirGoalieId_;
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

    PlayerID ourGoalieId_;
    PlayerID theirGoalieId_;

    GEARSystem::Controller &controller_;

    Sides::Side side_;
    Colors::Color color_;

    QMutex mutex_;
    QMutex refereeMutex_;
};

#endif