#ifndef NODE_HH
#define NODE_HH

#include <QString>

#include "types/status.hh"

namespace WRBeT {
class Node {
public:
    Node(QString name);

    QString name();
    virtual Status tick() = 0;

    Status status() const {
        return status_;
    }

    void assignParent(Node *parent) {
        parent_ = parent;
    }

protected:
    Node *parent_;
    Status status_;

private:
    QString name_;
};
} // namespace WRBeT
#endif // NODE_HH