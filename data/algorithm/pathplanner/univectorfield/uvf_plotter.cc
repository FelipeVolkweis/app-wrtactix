#include <QRandomGenerator>
#include <iostream>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <chrono>

using namespace std;

#include "algorithm/pathplanner/univectorfield/univectorfield.hh"
#include "constants/constants.hh"
#include "constants/config/config.hh"

#define MM2M 0.001

Vec2 generateRandomPoint() {
    float x = QRandomGenerator::global()->bounded(0, 9000) * MM2M - 4.5;
    float y = QRandomGenerator::global()->bounded(0, 6000) * MM2M - 3.0;
    return Vec2(x, y);
}

int main() {
    Config::initialize();
    Const::initialize();

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

    UnivectorField pf(Const::PathPlanner::UnivectorField::de,
                      Const::PathPlanner::UnivectorField::kr,
                      Const::PathPlanner::UnivectorField::ko,
                      Const::PathPlanner::UnivectorField::dmin,
                      Const::PathPlanner::UnivectorField::delta,
                      Const::PathPlanner::UnivectorField::maxIts,
                      Const::PathPlanner::UnivectorField::step);
    
    Vec2 origin = generateRandomPoint();
    Vec2 goal = generateRandomPoint();
    // Vec2 origin(3, 2.1);
    // Vec2 goal(6, 1.9);

    QVector<Obstacle> obstacles;
    // obstacles.push_back(Vec2(6, 2));
    // out << obstacles[0][0];
    // out << " ";
    // out << obstacles[0][1];
    // out << "\n";
    for (int i = 0; i < nObstacles; i++) {
        Vec2 o = generateRandomPoint();
        obstacles.push_back(Obstacle(o, 0.18));
        out << o[0];
        out << " ";
        out << o[1];
        out << "\n";
    }

    out2 << origin[0];
    out2 << " ";
    out2 << origin[1];
    out2 << "\n";

    out2 << goal[0];
    out2 << " ";
    out2 << goal[1];
    out2 << "\n";

    auto t1 = high_resolution_clock::now();
    auto path = pf.findPath(origin, goal, obstacles);
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