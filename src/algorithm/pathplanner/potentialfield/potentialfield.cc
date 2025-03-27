#include "algorithm/geometry/kdtree/kdtree.hh"

#include "potentialfield.hh"

#define EPS 1e-5

PotentialField::PotentialField(float kAtt, float kRep, float minRadiusForRepulsiveForce, float conicQuadraticThreshold,
                               float epsilon)
    : kAtt_(kAtt), kRep_(kRep), minRadiusForRepulsiveForce_(minRadiusForRepulsiveForce),
      conicQuadraticThreshold_(conicQuadraticThreshold), epsilon_(epsilon) {}

void PotentialField::addRepulsiveForce(const Vec2 &obstacle) {
    auto distance = (obstacle - origin_).norm() + EPS;
    if (distance > minRadiusForRepulsiveForce_) {
        return;
    }

    auto qq = 1.0f / minRadiusForRepulsiveForce_ + EPS;
    auto dd = 1.0f / distance;

    auto potentialForce = kRep_ * (qq - dd) * (1.0f / (distance * distance)) * (obstacle - origin_);

    addForce(potentialForce);
}

void PotentialField::addAttractiveForce() {
    auto distance = (goal_ - origin_).norm() + EPS;
    Vec2 potentialForce;
    if (distance <= conicQuadraticThreshold_) { // quadratic
        potentialForce = kAtt_ * (goal_ - origin_);
    } else { // conic
        potentialForce = kAtt_ * (goal_ - origin_).normalized() * conicQuadraticThreshold_;
    }

    addForce(potentialForce);
}

void PotentialField::addForce(const Vec2 &force) {
    resultantForce_ += force;
}

Vec2 PotentialField::getForce() const {
    return resultantForce_;
}

void PotentialField::reset() {
    resultantForce_ = Vec2(0, 0);
}

QVector<Vec2> PotentialField::findPath(const Vec2 &start, const Vec2 &end, const QVector<Obstacle> &obstacles) {
    QVector<Vec2> path;
    origin_ = start;
    goal_ = end;

    int it = 0;

    KDTree tree;
    for (auto &o : obstacles) {
        tree.insert(o.center);
    }

    Vec2 force;
    path.append(origin_);
    do {
        reset();
        for (const auto &obstacle : obstacles) {
            addRepulsiveForce(obstacle.center);
        }
        addAttractiveForce();

        force = getForce();

        origin_ += force.normalized() * stepSize_;
        path.append(origin_);
        it++;
    } while (resultantForce_.norm() > epsilon_ && it < maxIts_);

    return path;
}

float PotentialField::findGreedyPath(const Vec2 &start, const Vec2 &end, const QVector<Obstacle> &obstacles) {
    reset();

    origin_ = start;
    goal_ = end;

    for (const auto &obstacle : obstacles) {
        addRepulsiveForce(obstacle.center);
    }
    addAttractiveForce();

    return resultantForce_.norm();
}