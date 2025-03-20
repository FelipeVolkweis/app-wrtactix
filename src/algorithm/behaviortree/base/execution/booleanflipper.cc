#include "booleanflipper.hh"

using namespace WRBeT;

BooleanFlipper::BooleanFlipper(QString name, bool &flip) : Node(name), _flip(flip) {}

Status BooleanFlipper::tick() {
    _flip = !_flip;
    return Status::SUCCESS;
}