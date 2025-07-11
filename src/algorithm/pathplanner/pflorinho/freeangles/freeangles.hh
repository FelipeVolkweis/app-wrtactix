/***
 * Warthog Robotics
 * University of Sao Paulo (USP) at Sao Carlos
 * http://www.warthog.sc.usp.br/
 *
 * This file is part of WRCoach project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#ifndef FREEANGLES_H
#define FREEANGLES_H

#include "obstacle.hh"

#include <GEARSystem/gearsystem.hh>

namespace Lorinho {
class FreeAngles {
public:
    class Interval {
    private:
        float _angInitial;
        float _angFinal;
        bool _obstructed;

    public:
        Interval() {
            _angInitial = _angFinal = 0.0;
            _obstructed = false;
        }

        Interval(float angInitial, float angFinal, bool obstructed) {
            _angInitial = angInitial;
            _angFinal = angFinal;
            _obstructed = obstructed;
        }

        float angInitial() const {
            return _angInitial;
        }

        float angFinal() const {
            return _angFinal;
        }

        bool obstructed() const {
            return _obstructed;
        }
    };

private:
    static QList<Interval> _getFreeAngles(const Position &watcher, float initialPosAngle, float finalPosAngle,
                                          const QList<Obstacle> &obstacles, bool returnObstructed = false);

    static QList<Obstacle> calcObstaclesObstruction(const Position &watcher, const QList<Obstacle> &obstacles);

public:
    static QList<Interval> getFreeAngles(const Position &watcher, const Position &initialPos, const Position &finalPos,
                                         const QList<Obstacle> &obstacles, bool returnObstructed = false);

    static QList<Interval> getFreeAngles(const Position &watcher, float initialPosAngle, float finalPosAngle,
                                         const QList<Obstacle> &obstacles, bool returnObstructed = false);
};
} // namespace Lorinho
#endif // FREEANGLES_H
