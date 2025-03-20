#include <QRandomGenerator>
#include <iostream>
#include <QVector>

using namespace std;

#include "algorithm/pathplanner/potentialfield/potentialfield.hh"

int main() {
    float katt = 0.1;
    float krep = 1000000.0;
    float minRad = 1000.0;
    float threshhold = 90.0;
    float step = 100.0;
    float epsilon = 50.0;
    int maxIt = 100;

    int nObstacles = 12;

    PotentialField pf(katt, krep, minRad, threshhold);
    
    Vec2 origin(QRandomGenerator::global()->bounded(0, 9000), QRandomGenerator::global()->bounded(0, 6000));
    Vec2 goal(QRandomGenerator::global()->bounded(0, 9000), QRandomGenerator::global()->bounded(0, 6000));
    Vec2 force = pf.getForce();

    QVector<Vec2> obstacles;
    for (int i = 0; i < nObstacles; i++) {
        auto o = Vec2(QRandomGenerator::global()->bounded(0, 9000), QRandomGenerator::global()->bounded(0, 6000));
        obstacles.push_back(o);
        cout << o.transpose() << endl;
    }

    cout << endl;

    cout << origin.transpose() << endl;
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

        cout << origin.transpose() << endl;
        it++;
    }

    return 0;
}