#ifndef ASTAR_HH
#define ASTAR_HH

#include <QVector>
#include <QFile>

#include "astargrid.hh"
#include "types/vec2.hh"

class AStar {
public:
    class Node;

    AStar();
    QVector<Vec2> findPath(const Vec2 &start, const Vec2 &end, const QVector<Vec2> &obstacles);
    float h(const Node &a, const Node &b);

    void writeGridToFile(QFile *file);
private:
    QVector<Vec2> reconstructPath(const QHash<Node, Node> &cameFrom, const Node &current);
    float manhattanDistance(const Node &a, const Node &b);

    AStarGrid<90, 60, 10> grid_;
};

#endif