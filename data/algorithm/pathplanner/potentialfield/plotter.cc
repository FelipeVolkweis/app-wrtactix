#include <QRandomGenerator>
#include <iostream>
#include <QVector>
#include <QFile>
#include <QTextStream>

using namespace std;

#include "algorithm/pathplanner/potentialfield/potentialfield.hh"

#define MM2M 0.001

int main() {
    float katt = 1.0;
    float krep = 0.1;
    float minRad = 0.2;
    float threshhold = 0.5;
    float step = 0.1;
    float epsilon = 0.05;
    int maxIt = 1000;

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

    PotentialField pf(katt, krep, minRad, threshhold);
    
    Vec2 origin(QRandomGenerator::global()->bounded(0, 9000) * MM2M, QRandomGenerator::global()->bounded(0, 6000) * MM2M);
    Vec2 goal(QRandomGenerator::global()->bounded(0, 9000) * MM2M, QRandomGenerator::global()->bounded(0, 6000) * MM2M);
    // Vec2 origin(3, 2.1);
    // Vec2 goal(6, 1.9);
    Vec2 force = pf.getForce();

    QVector<Vec2> obstacles;
    // obstacles.push_back(Vec2(6, 2));
    // out << obstacles[0][0];
    // out << " ";
    // out << obstacles[0][1];
    // out << "\n";
    for (int i = 0; i < nObstacles; i++) {
        auto o = Vec2(QRandomGenerator::global()->bounded(0, 9000) * MM2M, QRandomGenerator::global()->bounded(0, 6000) * MM2M);
        obstacles.push_back(o);
        out << o[0];
        out << " ";
        out << o[1];
        out << "\n";
    }

    out2 << origin[0];
    out2 << " ";
    out2 << origin[1];
    out2 << "\n";

    int it = 0;
    QVector<Vec2> previousPositions;

    while ((goal - origin).norm() > epsilon && it < maxIt) {
        pf.reset();
        pf.setGoFromOriginToGoal(origin, goal);

        for (auto &o : obstacles) {
            pf.addRepulsiveForce(o);
        }

        pf.addAttractiveForce();

        force = pf.getForce();

        origin += force.normalized() * step;

        out2 << origin[0];
        out2 << " ";
        out2 << origin[1];
        out2 << "\n";
        it++;
    }

    out2 << goal[0];
    out2 << " ";
    out2 << goal[1];
    out2 << "\n";

    file.close();
    file2.close();

    return 0;
}