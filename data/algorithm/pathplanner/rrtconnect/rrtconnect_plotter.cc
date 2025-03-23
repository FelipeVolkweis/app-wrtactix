#include <QRandomGenerator>
#include <iostream>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <chrono>

using namespace std;

#include "algorithm/pathplanner/rrtconnect/rrtconnect.hh"

#define MM2M 0.001

Vec2 generateRandomPoint() {
    float x = QRandomGenerator::global()->bounded(0, 9000) * MM2M - 4.5;
    float y = QRandomGenerator::global()->bounded(0, 6000) * MM2M - 3.0;
    return Vec2(x, y);
}

int main() {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    int nObstacles = 100;

    QString filename("obstacles.dat");
    QString filename2("path.dat");

    QFile file(filename);
    QFile file2(filename2);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        cout << "Could not open file " << filename.toStdString() << endl;
        return 1;
    }

    if (!file2.open(QIODevice::WriteOnly | QIODevice::Text)) {
        cout << "Could not open file " << filename2.toStdString() << endl;
        return 1;
    }

    QTextStream out(&file);
    QTextStream out2(&file2);

    Vec2 origin = generateRandomPoint();
    Vec2 goal = generateRandomPoint();

    out2 << origin[0];
    out2 << " ";
    out2 << origin[1];
    out2 << "\n";

    out2 << goal[0];
    out2 << " ";
    out2 << goal[1];
    out2 << "\n";

    QVector<Vec2> obstacles;
    for (int i = 0; i < nObstacles; i++) {
        Vec2 o = generateRandomPoint();
        obstacles.push_back(o);
        out << o[0];
        out << " ";
        out << o[1];
        out << "\n";
    }
    auto t1 = high_resolution_clock::now();

    RRTConnect rrt;
    QVector<Vec2> path = rrt.findPath(origin, goal, obstacles);

    auto t2 = high_resolution_clock::now();

    cout << "Path size: " << path.size() << endl;
    duration<double, std::milli> ms_double = t2 - t1;
    cout << ms_double.count() << "ms\n";
    
    for (auto &p : path) {
        out2 << p[0];
        out2 << " ";
        out2 << p[1];
        out2 << "\n";
    }

    file.close();
    file2.close();

    return 0;
}