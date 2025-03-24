#include <QRandomGenerator>
#include <iostream>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <chrono>

using namespace std;

#include "algorithm/geometry/radialsweep/radialsweep.hh"

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
    AngleInterval ai(M_PI * 11.0f / 6.0f, M_PI / 6.0f);
    // AngleInterval ai(0.0f, M_2_PI_EXCLUSIVE);
    float radius = 3.0f;
    float obstacleRadius = 0.09f;

    QString filename("obstacles.dat");
    QString filename2("angles.dat");

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

    Vec2 observer = generateRandomPoint();

    QVector<Vec2> obstacles;
    for (int i = 0; i < nObstacles; i++) {
        Vec2 o = generateRandomPoint();
        obstacles.push_back(o);
        out << o[0];
        out << " ";
        out << o[1];
        out << "\n";
    }

    out2 << observer[0] << " " << observer[1] << "\n";
    out2 << ai.start.radians() << " " << ai.end.radians() << " " << radius << "\n";

    auto t1 = high_resolution_clock::now();
    RadialSweep rs(observer, obstacles, obstacleRadius, ai, radius);
    auto freeAngles = rs.getFreeAngles();
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> ms_double = t2 - t1;
    cout << ms_double.count() << "ms\n";

    for (const auto &fa : freeAngles) {
        out2 << fa.start.degrees();
        out2 << " ";
        out2 << fa.end.degrees();
        out2 << "\n";
    }

    file.close();
    file2.close();

    return 0;
}