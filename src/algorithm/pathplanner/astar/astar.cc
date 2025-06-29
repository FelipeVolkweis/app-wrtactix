#include <QHash>
#include <QTextStream>
#include <queue>

#include "astar.hh"

class AStar::Node {
public:
    int x, y;
    float g;
    float h;

    Node(int x = 0, int y = 0, float g = 0, float h = 0) : x(x), y(y), g(g), h(h) {}

    bool operator==(const Node &other) const {
        return x == other.x && y == other.y;
    }

    const float f() const {
        return g + h;
    }
};

AStar::AStar() {}

float AStar::h(const Node &a, const Node &b) {
    return manhattanDistance(a, b);
}

float AStar::manhattanDistance(const Node &a, const Node &b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

float AStar::chebyshevDistance(const Node &a, const Node &b) {
    return std::max(abs(a.x - b.x), abs(a.y - b.y));
}

float AStar::euclideanDistance(const Node &a, const Node &b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

float AStar::cost(const Node &a, const Node &b) {
    if (a.x == b.x || a.y == b.y) {
        return 1;
    } else {
        return 1.414;
    }
}

QVector<Vec2> AStar::findPath(const Vec2 &start, const Vec2 &end, const QVector<Obstacle> &obstacles) {
    QHash<Node, float> g;
    QHash<Node, Node> cameFrom;

    auto cmp = [](const Node &a, const Node &b) { return a.f() > b.f(); };
    using QPriorityQueue = std::priority_queue<Node, std::vector<Node>, decltype(cmp)>;
    QPriorityQueue open(cmp);

    auto tmpStart = grid_.convertVec2ToGrid(start);
    auto tmpEnd = grid_.convertVec2ToGrid(end);
    Node startNode(tmpStart.first, tmpStart.second);
    Node endNode(tmpEnd.first, tmpEnd.second);
    startNode.h = h(startNode, endNode);

    grid_.clearGrid();

    for (auto &obstacle : obstacles) {
        grid_.setObstacle(obstacle.center, obstacle.radius);
    }

    open.push(startNode);
    g[startNode] = 0;

    QVector<QPair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    Node closestNode = startNode;
    float closestDistance = h(startNode, endNode);
    int its = 0;
    while (!open.empty() && its < maxIts_) {
        auto current = open.top();
        open.pop();
        its++;

        float currentDistance = h(current, endNode);
        if (currentDistance < closestDistance) {
            closestNode = current;
            closestDistance = currentDistance;
        }

        if (current == endNode) {
            return reconstructPath(cameFrom, current, start, end);
        }

        if (current.g > g[current])
            continue;

        for (const auto &d : directions) {
            auto x = current.x + d.first;
            auto y = current.y + d.second;

            if (x < 0 || x >= grid_.width() || y < 0 || y >= grid_.height()) {
                continue;
            }

            if (grid_.isObstacle(x, y)) {
                continue;
            }

            Node neighbor(x, y);
            float tentativeG = g[current] + cost(current, neighbor);

            if (!g.contains(neighbor) || tentativeG < g[neighbor]) {
                g[neighbor] = tentativeG;
                neighbor.g = tentativeG;
                neighbor.h = h(neighbor, endNode);

                open.push(neighbor);
                cameFrom[neighbor] = current;
            }
        }
    }
    return reconstructPath(cameFrom, closestNode);
}

QVector<Vec2> AStar::reconstructPath(const QHash<Node, Node> &cameFrom, const Node &current) {
    QVector<Vec2> path;
    Node tmp = current;

    while (cameFrom.contains(tmp)) {
        path.push_front(grid_.convertGridToVec2(tmp.x, tmp.y));
        tmp = cameFrom[tmp];
    }

    path.push_front(grid_.convertGridToVec2(tmp.x, tmp.y));

    return path;
}

QVector<Vec2> AStar::reconstructPath(const QHash<Node, Node> &cameFrom, const Node &current, const Vec2 &start,
                                     const Vec2 &end) {
    QVector<Vec2> path;
    Node tmp = current;

    while (cameFrom.contains(tmp)) {
        path.push_front(grid_.convertGridToVec2(tmp.x, tmp.y));
        tmp = cameFrom[tmp];
    }
    if (path.isEmpty()) {
        return path;
    }
    path[0] = start;
    path[path.size() - 1] = end;

    return path;
}

inline size_t qHash(const AStar::Node &key, size_t seed = 0) {
    return qHash(key.x, seed) ^ qHash(key.y, seed);
}

void AStar::writeGridToFile(QFile *file) {
    if (!file->isOpen() && !file->open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream out(file);

    for (int i = 0; i < grid_.width(); i++) {
        for (int j = 0; j < grid_.height(); j++) {
            if (grid_.isObstacle(i, j)) {
                out << i << " " << j << "\n";
            }
        }
    }

    file->close();
}