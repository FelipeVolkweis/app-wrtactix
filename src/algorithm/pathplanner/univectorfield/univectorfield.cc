#include "univectorfield.hh"

UnivectorField::UnivectorField() {}

UnivectorField::UnivectorField(float de, float kr, float ko, float dmin, float delta, float maxits, float step)
    : de_(de), kr_(kr), ko_(ko), dMin_(dmin), delta_(delta), maxIts_(maxits), step_(step) {}

QVector<Vec2> UnivectorField::findPath(const Vec2 &start, const Vec2 &goal, const QVector<Obstacle> &obstacles) {
    QVector<Vec2> path;
    Vec2 originPos = start;
    Vec2 goalPos = goal;

    path = generatePath(originPos, goalPos, obstacles, Vec2(0, 0), Vec2(0, 0));

    return path;
}

// Wrap angle to [-π, π]
float UnivectorField::wrapToPi(float angle) {
    while (angle > M_PI)
        angle -= 2 * M_PI;
    while (angle < -M_PI)
        angle += 2 * M_PI;
    return angle;
}

float UnivectorField::atan2Pi(float y, float x) {
    return wrapToPi(atan2f(y, x));
}

float UnivectorField::gaussian(float r, float delta) {
    return expf(-(r * r) / (2 * delta * delta));
}

float UnivectorField::phiH(float rho, float theta, float de, float kr, bool cw = false) {
    float angle;

    if (rho > de) {
        angle = (M_PI / 2) * (2 - ((de + kr) / (rho + kr)));
    } else { // 0 <= rho <= radius
        angle = (M_PI / 2) * sqrtf(rho / de);
    }

    return wrapToPi(cw ? theta + angle : theta - angle);
}

float UnivectorField::phiR(const Vec2 &delta) {
    return atan2Pi(delta.y(), delta.x());
}

float UnivectorField::phiTuf(float theta, const Vec2 &deltaGoal, float de) {
    float yL = deltaGoal.y() + de;
    float yR = deltaGoal.y() - de;

    Vec2 pL(deltaGoal.x(), deltaGoal.y() - de);
    Vec2 pR(deltaGoal.x(), deltaGoal.y() + de);

    float phiCcw = phiH(pL.norm(), theta, de, kr_, false);
    float phiCw = phiH(pR.norm(), theta, de, kr_, true);

    Vec2 nhCcw(cosf(phiCcw), sinf(phiCcw));
    Vec2 nhCw(cosf(phiCw), sinf(phiCw));

    Vec2 spiralMerge = (fabsf(yL) * nhCcw + fabsf(yR) * nhCw) / (2 * de);

    if (-de <= deltaGoal.y() && deltaGoal.y() < de) {
        return atan2Pi(spiralMerge.y(), spiralMerge.x());
    } else if (deltaGoal.y() < -de) {
        return phiH(pL.norm(), theta, de, kr_, true);
    } else {
        return phiH(pR.norm(), theta, de, kr_, false);
    }
}

float UnivectorField::phiAuf(const Vec2 &obstaclePos, const Vec2 &robotPos, const Vec2 &vObs, const Vec2 &vRob,
                             float ko) {
    Vec2 sVec = ko * (vObs - vRob);
    float sNorm = sVec.norm();
    float obsRobotDist = (obstaclePos - robotPos).norm();

    Vec2 pLineObs;
    if (obsRobotDist >= sNorm) {
        pLineObs = obstaclePos + sVec;
    } else {
        pLineObs = obstaclePos + sVec * (obsRobotDist / sNorm);
    }

    Vec2 delta = robotPos - pLineObs;
    return phiR(delta);
}

float UnivectorField::phiComposed(float phiTuf, float phiAuf, float R, const QVector<Obstacle> &obstacles, float delta,
                                  float dMin) {
    if (obstacles.empty()) {
        return wrapToPi(phiTuf);
    }

    float g = gaussian(R - dMin, delta);

    if (R <= dMin) {
        return phiAuf;
    } else {
        float diff = wrapToPi(phiAuf - phiTuf);
        return wrapToPi(g * diff + phiTuf);
    }
}

Vec2 UnivectorField::closestObstacle(const Vec2 &robotPos, const QVector<Obstacle> &obstacles) {
    if (obstacles.empty()) {
        return NULL_VEC;
    }

    Vec2 closest = getObstacleSurface(obstacles[0], robotPos);
    float minDist = (robotPos - closest).norm();

    for (const auto &obstacle : obstacles) {
        auto obs = getObstacleSurface(obstacle, robotPos);
        float dist = (robotPos - obs).norm();
        if (dist < minDist) {
            minDist = dist;
            closest = obs;
        }
    }

    return closest;
}

Vec2 UnivectorField::getDirection(const Vec2 &originPos, const Vec2 &goalPos, const QVector<Obstacle> &obstacles,
                                  const Vec2 &vObs, const Vec2 &vRob) {
    Vec2 deltaGoal = originPos - goalPos;
    float theta = phiR(deltaGoal);
    float phi_tuf = phiTuf(theta, deltaGoal, de_);

    if (obstacles.empty()) {
        return Vec2(cosf(phi_tuf), sinf(phi_tuf));
    }

    Vec2 closestObs = closestObstacle(originPos, obstacles);
    float R = (closestObs - originPos).norm();
    float robotObsDist = R;

    float phi_auf = phiAuf(closestObs, originPos, vObs, vRob, ko_);
    float phi_composed = phiComposed(phi_tuf, phi_auf, R, obstacles, delta_, dMin_);

    return Vec2(cosf(phi_composed), sinf(phi_composed));
}

Vec2 UnivectorField::getDirection(const Vec2 &originPos, const Vec2 &goalPos, const QVector<Obstacle> &obstacles) {
    return getDirection(originPos, goalPos, obstacles, Vec2(0, 0), Vec2(0, 0));
}

QVector<Vec2> UnivectorField::generatePath(const Vec2 &originPos, const Vec2 &goalPos,
                                           const QVector<Obstacle> &obstacles, const Vec2 &vObs, const Vec2 &vRob) {
    QVector<Vec2> path;
    Vec2 currentPos = originPos;
    path.push_back(currentPos);

    float distanceToGoal;
    int iterations = 0;

    do {
        Vec2 direction = getDirection(currentPos, goalPos, obstacles, vObs, vRob);

        currentPos = currentPos + direction * step_;
        path.push_back(currentPos);

        distanceToGoal = (goalPos - currentPos).norm();

        if (iterations++ > maxIts_) {
            break;
        }

    } while (distanceToGoal > step_ * 1.2f);

    return path;
}

Vec2 UnivectorField::getObstacleSurface(const Obstacle &obstacle, const Vec2 &origin) {
    Vec2 obstacleSurface;
    if (obstacle.type == ObstacleType::CIRCLE) {
        if ((obstacle.center - origin).norm() > obstacle.radius) {
            obstacleSurface = obstacle.center + obstacle.radius * (origin - obstacle.center).normalized();
        } else {
            obstacleSurface = obstacle.center;
        }
    } else if (obstacle.type == ObstacleType::RECTANGLE) {
        // Precisa validar
        float x = std::clamp(origin.x(), obstacle.bottomLeft.x(), obstacle.topRight.x());
        float y = std::clamp(origin.y(), obstacle.bottomLeft.y(), obstacle.topRight.y());
        obstacleSurface = Vec2{x, y};
    }

    return obstacleSurface;
}