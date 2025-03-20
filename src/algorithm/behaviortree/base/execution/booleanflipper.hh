#ifndef BOOLEANFLIPPER_HH
#define BOOLEANFLIPPER_HH

#include "algorithm/behaviortree/node.hh"

namespace WRBeT {
class BooleanFlipper : public Node {
public:
    BooleanFlipper(QString name, bool &flip);

    Status tick();

private:
    bool &_flip;
};
} // namespace WRBeT
#endif // BOOLEANFLIPPER_HH