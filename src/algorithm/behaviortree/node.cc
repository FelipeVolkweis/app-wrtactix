#include "node.hh"

using namespace WRBeT;

Node::Node(QString name) : _name(name) {}

QString Node::name() {
    return _name;
}