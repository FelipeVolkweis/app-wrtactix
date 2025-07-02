#ifndef LOCATIONS_HH
#define LOCATIONS_HH

#include "world/world.hh"

class Locations {
public:
    Locations(const World &world);

    bool isInOurTopQuadrant(const Position &pos) const {
        return _world.ourSide() == Sides::LEFT ? isInTopLeftQuadrant(pos) : isInTopRightQuadrant(pos);
    }

    bool isInOurBottomQuadrant(const Position &pos) const {
        return _world.ourSide() == Sides::LEFT ? isInBottomLeftQuadrant(pos) : isInBottomRightQuadrant(pos);
    }

    bool isInTheirTopQuadrant(const Position &pos) const {
        return _world.theirSide() == Sides::LEFT ? isInTopLeftQuadrant(pos) : isInTopRightQuadrant(pos);
    }

    bool isInTheirBottomQuadrant(const Position &pos) const {
        return _world.theirSide() == Sides::LEFT ? isInBottomLeftQuadrant(pos) : isInBottomRightQuadrant(pos);
    }

    bool isInCenterCircle(const Position &pos) const {
        float r = _world.fieldCenterRadius();
        return pos.x() < r && pos.x() > -r && pos.y() < r && pos.y() > -r;
    }

    bool isInOurGoalArea(const Position &pos) const {
        return _world.ourSide() == Sides::LEFT ? isInLeftGoalArea(pos) : isInRightGoalArea(pos);
    }

    bool isInTheirGoalArea(const Position &pos) const {
        return _world.theirSide() == Sides::LEFT ? isInLeftGoalArea(pos) : isInRightGoalArea(pos);
    }

    bool isInOurFieldSide(const Position &pos) const {
        return _world.ourSide() == Sides::LEFT ? isInLeftFieldSide(pos) : isInRightFieldSide(pos);
    }

    bool isInTheirFieldSide(const Position &pos) const {
        return _world.theirSide() == Sides::LEFT ? isInLeftFieldSide(pos) : isInRightFieldSide(pos);
    }

    bool isInTheField(const Position &pos) const {
        return isInTheirFieldSide(pos) || isInOurFieldSide(pos);
    }

    bool isInTheField(const Position &pos, float tolerance) const;

    Goal ourGoal() const {
        return _world.ourSide() == Sides::LEFT ? _world.leftGoal() : _world.rightGoal();
    }

    Goal theirGoal() const {
        return _world.theirSide() == Sides::LEFT ? _world.leftGoal() : _world.rightGoal();
    }

    Position ourPenaltyMark() const {
        return _world.ourSide() == Sides::LEFT ? _world.leftPenaltyMark() : _world.rightPenaltyMark();
    }

    Position theirPenaltyMark() const {
        return _world.theirSide() == Sides::LEFT ? _world.leftPenaltyMark() : _world.rightPenaltyMark();
    }

    Position ourGoalCenter() const {
        Position goal(
            true, _world.ourSide() == Sides::LEFT ? _world.fieldTopLeftCorner().x() : _world.fieldTopRightCorner().x(),
            0, 0);
        return goal;
    }

    Position theirGoalCenter() const {
        Position goal(true,
                      _world.theirSide() == Sides::LEFT ? _world.fieldTopLeftCorner().x()
                                                        : _world.fieldTopRightCorner().x(),
                      0, 0);
        return goal;
    }

private:
    bool isInTopLeftQuadrant(const Position &pos) const {
        return pos.x() <= 0 && pos.y() >= 0;
    }

    bool isInTopRightQuadrant(const Position &pos) const {
        return pos.x() >= 0 && pos.y() >= 0;
    }

    bool isInBottomLeftQuadrant(const Position &pos) const {
        return pos.x() <= 0 && pos.y() <= 0;
    }

    bool isInBottomRightQuadrant(const Position &pos) const {
        return pos.x() >= 0 && pos.y() <= 0;
    }

    bool isInLeftGoalArea(const Position &pos) const {
        /*
            +----+ p1
            |    |
            |    |
            |    |
            +----+ p2
        */
        float p1x = _world.fieldBottomLeftCorner().x() + _world.leftGoal().getAreaWidth();
        float p1y = _world.leftGoal().getAreaLength() / 2;
        float p2y = -p1y;

        return pos.x() <= p1x && pos.y() <= p1y && pos.y() >= p2y;
    }

    bool isInRightGoalArea(const Position &pos) const {
        /*
            p1 +----+
               |    |
               |    |
               |    |
            p2 +----+
        */
        float p1x = _world.fieldBottomRightCorner().x() - _world.rightGoal().getAreaWidth();
        float p1y = _world.rightGoal().getAreaLength() / 2;
        float p2y = -p1y;

        return pos.x() >= p1x && pos.y() <= p1y && pos.y() >= p2y;
    }

    bool isInLeftFieldSide(const Position &pos) const {
        return pos.x() <= 0 && pos.x() >= _world.fieldBottomLeftCorner().x();
    }

    bool isInRightFieldSide(const Position &pos) const {
        return pos.x() >= 0 && pos.x() <= _world.fieldBottomRightCorner().x();
    }

    const World &_world;
};

#endif