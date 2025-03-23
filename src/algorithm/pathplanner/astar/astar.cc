#include <QHash>
#include <QTextStream>
#include <queue>

#include "astar.hh"

class AStar::Node {
public:
    int x, y; 
    float g;
    float h;

    Node(int x = 0, int y = 0, float g = 0, float h = 0)
        : x(x), y(y), g(g), h(h) {}

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

QVector<Vec2> AStar::findPath(const Vec2 &start, const Vec2 &end, const QVector<Vec2> &obstacles) {
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
        grid_.setObstacle(obstacle, 0.09);
    }

    open.push(startNode);
    g[startNode] = 0;

    QVector<QPair<int, int>> directions = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0},
    };

    while (!open.empty()) {
        auto current = open.top();

        if (current == endNode) {
            return reconstructPath(cameFrom, current);
        }
        open.pop();

        if (current.g > g[current]) continue; 

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
            float tentativeG = g[current] + 1;

            if (!g.contains(neighbor) || tentativeG < g[neighbor]) {
                g[neighbor] = tentativeG;
                neighbor.g = tentativeG;
                neighbor.h = h(neighbor, endNode);

                open.push(neighbor);
                cameFrom[neighbor] = current;
            }
        }
    }

    return QVector<Vec2>();
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
                out << 1 << " ";
            } else {
                out << 0 << " ";
            }
        }
        out << "\n";
    }

    file->close();
}