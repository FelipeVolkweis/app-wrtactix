#include "followball.hh"

FollowBall::FollowBall(const PlayerID &player, SSLController &controller, const World &world)
    : SSLBehavior(player, controller, world, "FollowBall") {}