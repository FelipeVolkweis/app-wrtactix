#ifndef ROTATEAROUND_HH
#define ROTATEAROUND_HH

#include "agent/sslagent/sslaction.hh"

#include <functional>

class RotateAround : public SSLAction {
public:
    RotateAround(const PlayerID &player, SSLController &controller, const World &world);

    RotateAround *setPoint(std::function<Vec2()> point);
    RotateAround *setLookAt(std::function<Vec2()> lookAt);
    RotateAround *setRadius(float r);
    
    Status execute();

private:
    std::function<Vec2()> point_;
    std::function<Vec2()> lookAt_;
    float radius_;
};

#endif 