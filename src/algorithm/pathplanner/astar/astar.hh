#ifndef ASTAR_HH
#define ASTAR_HH

#include <QFile>

#include "algorithm/pathplanner/pathplanner.hh"

#include "astargrid.hh"

class AStar : public PathPlanner {
public:
    class Node;

    AStar();
    QVector<Vec2> findPath(const Vec2 &start, const Vec2 &end, const QVector<Obstacle> &obstacles);
    float h(const Node &a, const Node &b);

    void writeGridToFile(QFile *file);

private:
    QVector<Vec2> reconstructPath(const QHash<Node, Node> &cameFrom, const Node &current);
    QVector<Vec2> reconstructPath(const QHash<Node, Node> &cameFrom, const Node &current, const Vec2 &start,
                                  const Vec2 &end);
    float manhattanDistance(const Node &a, const Node &b);
    float chebyshevDistance(const Node &a, const Node &b);
    float euclideanDistance(const Node &a, const Node &b);

    float cost(const Node &a, const Node &b);

    AStarGrid<90, 60, 10> coarseGrid_;
    // AStarGrid<9000, 6000, 1000> fineGrid_;

    const int maxIts_ = 1000;
};

#endif