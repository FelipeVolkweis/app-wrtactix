#include "locations.hh"

Locations::Locations(const World &world) : _world(world) {}

bool Locations::isInTheField(const Position &pos, float tolerance) const {
    Position bottomLeft(true, _world.fieldBottomLeftCorner().x() - tolerance,
                        _world.fieldBottomLeftCorner().y() - tolerance, 0.0);
    Position topRight(true, _world.fieldTopRightCorner().x() + tolerance, _world.fieldTopRightCorner().y() + tolerance,
                      0.0);

    return pos.x() > bottomLeft.x() && pos.x() < topRight.x() && pos.y() > bottomLeft.y() && pos.y() < topRight.y();
}