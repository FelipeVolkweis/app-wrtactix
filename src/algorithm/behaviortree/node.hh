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

protected:
    Node *_parent;

private:
    QString _name;
};
} // namespace WRBeT
#endif // NODE_HH