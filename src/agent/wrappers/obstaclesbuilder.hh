#ifndef OBSTACLESBUILDER_HH
#define OBSTACLESBUILDER_HH

#include "types/obstacle.hh"
#include "world/world.hh"

class ObstaclesBuilder {
public: 
    ObstaclesBuilder(const World &world, const PlayerID &id) : world_(world), id_(id) {}

    void avoidTeammates();
    void avoidOpponents();
    void avoidBall();
    void avoidOurGoal();
    void avoidTheirGoal();
    void reset();

    const QVector<Obstacle> &obstacles() const {
        return obstacles_;
    }

private:
    const PlayerID &id_;
    const World &world_;

    QVector<Obstacle> obstacles_;
};

#endif