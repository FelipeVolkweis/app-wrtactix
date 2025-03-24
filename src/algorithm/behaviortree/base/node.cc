#include "node.hh"

using namespace WRBeT;

Node::Node(QString name) : name_(name), parent_(nullptr) {}

QString Node::name() {
    return name_;
}