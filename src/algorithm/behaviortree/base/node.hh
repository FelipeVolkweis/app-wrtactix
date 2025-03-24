#ifndef NODE_HH
#define NODE_HH

#include <QString>

namespace WRBeT {
enum Status { SUCCESS, FAILURE, RUNNING };

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