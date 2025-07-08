#ifndef BLOCK_HH
#define BLOCK_HH

#include "world/world.hh"

class Block {
public:
    Block(const World &world);

    bool hitsOurGoal(float m, float b) const;
    Vec2 getGoaliePosition() const;
    Vec2 getKickOutOfOurArea(const PlayerID &callerId) const;
    Vec2 getBarrierPosition() const;

private:
    struct Line {
        float m;
        float b;
        bool vertical;
        float x0;
    };

    const World &world_;

    Line getBallImpactLine() const;
    QPair<Vec2, float> getGoalieCircumference() const;
    Vec2 getGoaliePositionInCircumference(float m, float b) const;
};

#endif