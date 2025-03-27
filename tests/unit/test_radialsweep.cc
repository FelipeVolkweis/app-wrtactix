#include "utils/testsuite/testsuite.hh"
#include "algorithm/geometry/radialsweep/radialsweep.hh"

#include <iostream>

class TestRadialSweep : public TestSuite {
    Q_OBJECT
public:
    using TestSuite::TestSuite;

private slots:
    void testCreateAngleEvents(); 
    void testMergeObstructedAngles();
    void testMergeAfterCreation();
    void testGetComplementIntervals();
    void testHalfOutsideInterval();
    void testEmptyObstacles();
    void testBoundaryIntervals();
    void testAllObstructed();
    void testPartialWrappedInterval();
    void testPartialInterval();
    void testPartialIntervalWithObstacleOn4th();
    void testPartialNoObstacles();
    void testLargestInterval();
    void testCenterOfInterval();
};

void TestRadialSweep::testCreateAngleEvents() {
    Vec2 observer(0, 0);
    QVector<Vec2> obstacles = { Vec2(1, 0) };
    AngleInterval interval(0, M_2_PI_EXCLUSIVE);
    float radius = 2.0;
    float obstacleRadius = 0.5;

    QVector<AngleEvent> events = RadialSweep::createEvents(obstacles, obstacleRadius, observer, radius, interval);
    int last = events.size() - 1;

    QVERIFY(events.size() == 4);
    QVERIFY(events[0].angle == WRAngle(0));
    QVERIFY(events[0].start == true);
    QVERIFY(events[last].angle == WRAngle(M_2_PI_EXCLUSIVE));
    QVERIFY(events[last].start == false);
}

void TestRadialSweep::testMergeObstructedAngles() {
    AngleEvent e1 = { WRAngle(0.1f), true };
    AngleEvent e2 = { WRAngle(0.2f), true };
    AngleEvent e3 = { WRAngle(0.3f), false };
    AngleEvent e4 = { WRAngle(0.4f), false };

    QVector<AngleEvent> events = { e1, e2, e3, e4 };    // Obstacle at 0 radians, so interval should cover [0, α) and [2π - α, 2π)

    QVector<AngleInterval> intervals = RadialSweep::mergeObstructedIntervals(events);

    QVERIFY(intervals.size() == 1);
    QVERIFY(intervals[0].start == e1.angle);
    QVERIFY(intervals[0].end == e4.angle);
}

void TestRadialSweep::testMergeAfterCreation() {
    Vec2 observer(0, 0);
    QVector<Vec2> obstacles = { Vec2(1, 0), Vec2(0, 1), Vec2(1, 1) };
    AngleInterval interval(0, M_2_PI_EXCLUSIVE);
    float radius = 2.0f;
    float obstacleRadius = 0.5f;

    QVector<AngleEvent> events = RadialSweep::createEvents(obstacles, obstacleRadius, observer, radius, interval);
    QVector<AngleInterval> intervals = RadialSweep::mergeObstructedIntervals(events);

    QVERIFY(events.size() > 0);
    QVERIFY(intervals.size() > 0);

    int n = intervals.size() - 1;
    QVERIFY(intervals[0].start == WRAngle(0));
    QVERIFY(intervals[0].end == WRAngle(2.0f * M_PI / 3.0f)); // 120 degrees
    QVERIFY(intervals[n].start == WRAngle(M_PI * 11.0f/6.0f)); // 330 degrees
    QVERIFY(intervals[n].end == WRAngle(M_2_PI_EXCLUSIVE));
}

void TestRadialSweep::testGetComplementIntervals() {
    QVector<AngleInterval> intervals = {
        { WRAngle(0), WRAngle(M_PI / 2.0f) },
        { WRAngle(M_PI), WRAngle(3.0f * M_PI / 2.0f) },
    };
    AngleInterval interval(0, M_2_PI_EXCLUSIVE);

    QVector<AngleInterval> complement = RadialSweep::getComplementIntervals(intervals, interval);

    QVERIFY(complement.size() == 2);
    QVERIFY(complement[0].start == WRAngle(M_PI / 2.0f));
    QVERIFY(complement[0].end == WRAngle(M_PI));
    QVERIFY(complement[1].start == WRAngle(3.0f * M_PI / 2.0f));
    QVERIFY(complement[1].end == WRAngle(M_2_PI_EXCLUSIVE));

    intervals = {
        {WRAngle(0), WRAngle(M_PI * 1.0f / 6.0f)},
        {WRAngle(M_PI * 11.0f / 6.0f), WRAngle(M_2_PI_EXCLUSIVE)}
    };

    complement = RadialSweep::getComplementIntervals(intervals, interval);

    QVERIFY(complement.size() == 1);
    QVERIFY(complement[0].start == WRAngle(M_PI * 1.0f / 6.0f));
    QVERIFY(complement[0].end == WRAngle(M_PI * 11.0f / 6.0f));
}

void TestRadialSweep::testHalfOutsideInterval() {
    Vec2 observer(0, 0);
    QVector<Vec2> obstacles = { Vec2(1, 0), Vec2(0, 1)};
    AngleInterval interval(M_PI / 2.0f, M_2_PI_EXCLUSIVE);
    float radius = 2.0;
    float obstacleRadius = 0.5;

    QVector<AngleEvent> events = RadialSweep::createEvents(obstacles, obstacleRadius, observer, radius, interval);
    QVector<AngleInterval> intervals = RadialSweep::mergeObstructedIntervals(events);
    QVector<AngleInterval> complement = RadialSweep::getComplementIntervals(intervals, interval);

    QVERIFY(intervals.size() == 2);

    int n = intervals.size() - 1;
    QVERIFY(intervals[0].start == WRAngle(M_PI / 2.0f));
    QVERIFY(intervals[0].end == WRAngle(M_PI * 2.0f / 3.0f));
    QVERIFY(intervals[n].start == WRAngle(M_PI * 11.0f / 6.0f));
    QVERIFY(intervals[n].end == WRAngle(M_2_PI_EXCLUSIVE));

    QVERIFY(complement.size() == 1);
    QVERIFY(complement[0].start == WRAngle(M_PI * 2.0f / 3.0f));
    QVERIFY(complement[0].end == WRAngle(M_PI * 11.0f / 6.0f));
}

void TestRadialSweep::testEmptyObstacles() {
    Vec2 observer(0, 0);
    QVector<Vec2> obstacles;
    AngleInterval interval(0, M_2_PI_EXCLUSIVE);
    float radius = 2.0f;
    float obstacleRadius = 0.5f;

    QVector<AngleEvent> events = RadialSweep::createEvents(obstacles, obstacleRadius, observer, radius, interval);
    QVector<AngleInterval> intervals = RadialSweep::mergeObstructedIntervals(events);
    QVector<AngleInterval> complement = RadialSweep::getComplementIntervals(intervals, interval);

    // No obstacles, so no events or intervals
    QVERIFY(events.empty());
    QVERIFY(intervals.empty());

    // Complement should be the entire interval
    QVERIFY(complement.size() == 1);
    QVERIFY(complement[0].start == WRAngle(0));
    QVERIFY(complement[0].end == WRAngle(M_2_PI_EXCLUSIVE));
}

void TestRadialSweep::testBoundaryIntervals() {
    Vec2 observer(0, 0);
    QVector<Vec2> obstacles = { Vec2(1, 0), Vec2(-1, 0) };
    AngleInterval interval(0, M_2_PI_EXCLUSIVE);
    float radius = 2.0f;
    float obstacleRadius = 0.5f;

    QVector<AngleEvent> events = RadialSweep::createEvents(obstacles, obstacleRadius, observer, radius, interval);
    QVector<AngleInterval> intervals = RadialSweep::mergeObstructedIntervals(events);
    QVector<AngleInterval> complement = RadialSweep::getComplementIntervals(intervals, interval);

    // Obstacles at 0 and π radians, so intervals should cover [0, α) and [π - α, π + α)
    QVERIFY(intervals.size() == 3);
    QVERIFY(intervals[0].start == WRAngle(0));
    QVERIFY(intervals[0].end == WRAngle(M_PI / 6)); // 30 degrees
    QVERIFY(intervals[1].start == WRAngle(5 * M_PI / 6)); // 150 degrees
    QVERIFY(intervals[1].end == WRAngle(7 * M_PI / 6)); // 210 degrees

    // Complement should have gaps between intervals
    QVERIFY(complement.size() == 2);
    QVERIFY(complement[0].start == WRAngle(M_PI / 6));
    QVERIFY(complement[0].end == WRAngle(5 * M_PI / 6));
    QVERIFY(complement[1].start == WRAngle(7 * M_PI / 6));
    QVERIFY(complement[1].end == WRAngle(M_PI * 11.0 / 6.0));
}

void TestRadialSweep::testAllObstructed() {
    Vec2 observer(0, 0);
    QVector<Vec2> obstacles = { Vec2(1, 0), Vec2(0, 1), Vec2(-1, 0), Vec2(0, -1) };
    AngleInterval interval(0, M_2_PI_EXCLUSIVE);
    float radius = 2.0f;
    float obstacleRadius = 1.0f;

    QVector<AngleEvent> events = RadialSweep::createEvents(obstacles, obstacleRadius, observer, radius, interval);
    QVector<AngleInterval> intervals = RadialSweep::mergeObstructedIntervals(events);
    QVector<AngleInterval> complement = RadialSweep::getComplementIntervals(intervals, interval);
    
    QVERIFY(intervals.size() == 1);

    // Complement should be empty
    QVERIFY(complement.empty());
}

void TestRadialSweep::testPartialWrappedInterval() {
    Vec2 observer(0, 0);
    QVector<Vec2> obstacles = { Vec2(1, 0) };
    AngleInterval interval(M_PI * 3.0f / 2.0f, M_PI / 2.0f);
    float radius = 3.0f;
    float obstacleRadius = 0.5f;

    RadialSweep rs = RadialSweep(observer, obstacles, obstacleRadius, interval, radius);
    QVector<AngleInterval> freeAngles = rs.getFreeAngles();  

    QVERIFY(freeAngles.size() == 2);
    QVERIFY(freeAngles[0].start == WRAngle(M_PI * 3.0f / 2.0f));
    QVERIFY(freeAngles[0].end == WRAngle(M_PI * 11.0f / 6.0f));
    QVERIFY(freeAngles[1].start == WRAngle(M_PI / 6.0f));
    QVERIFY(freeAngles[1].end == WRAngle(M_PI / 2.0f));
}

void TestRadialSweep::testPartialInterval() {
    Vec2 observer(0, 0);
    QVector<Vec2> obstacles = { Vec2(-1, 0) };
    AngleInterval interval(M_PI / 2.0f, M_PI * 3.0f / 2.0f);
    float radius = 3.0f;
    float obstacleRadius = 0.5f;

    RadialSweep rs(observer, obstacles, obstacleRadius, interval, radius);
    QVector<AngleInterval> freeAngles = rs.getFreeAngles();
    QVector<AngleInterval> obstructedAngles = rs.getObstructedAngles();

    QVERIFY(freeAngles.size() == 2);
    QVERIFY(freeAngles[0].start == WRAngle(M_PI / 2.0f));
    QVERIFY(freeAngles[0].end == WRAngle(5.0f * M_PI / 6.0f));
}

void TestRadialSweep::testPartialIntervalWithObstacleOn4th() {
    Vec2 observer(0, 0);
    QVector<Vec2> obstacles = { Vec2(1, -1) };
    AngleInterval interval(M_PI * 3.0f / 2.0f, M_PI / 2.0f);
    float radius = 3.0f;
    float obstacleRadius = 0.5f;

    RadialSweep rs(observer, obstacles, obstacleRadius, interval, radius);
    QVector<AngleInterval> freeAngles = rs.getFreeAngles();
    QVector<AngleInterval> obstructedAngles = rs.getObstructedAngles();

    QVERIFY(freeAngles.size() == 3);
    QVERIFY(freeAngles[0].start == WRAngle(M_PI * 3.0f / 2.0f));
    QVERIFY(freeAngles[0].end == WRAngle(atan2f(-1.2057189138831, 0.5442810861169)));
    QVERIFY(freeAngles[1].start == WRAngle(atan2f(-0.5442810861169, 1.2057189138831))); // (1.2057189138831, -0.5442810861169)
    QVERIFY(freeAngles[1].end == WRAngle(M_2_PI_EXCLUSIVE)); 
    QVERIFY(freeAngles[2].start == WRAngle(0));
    QVERIFY(freeAngles[2].end == WRAngle(M_PI / 2.0f));
}

void TestRadialSweep::testPartialNoObstacles() {
    Vec2 observer(0, 0);
    QVector<Vec2> obstacles = { };
    AngleInterval interval(M_PI * 3.0f / 2.0f, M_PI / 2.0f);
    float radius = 3.0f;
    float obstacleRadius = 0.5f;

    {
        RadialSweep rs(observer, obstacles, obstacleRadius, interval, radius);
        QVector<AngleInterval> freeAngles = rs.getFreeAngles();
        QVector<AngleInterval> obstructedAngles = rs.getObstructedAngles();

        QVERIFY(freeAngles.size() == 2);
        QVERIFY(freeAngles[0].start == WRAngle(M_PI * 3.0f / 2.0f));
        QVERIFY(freeAngles[0].end == WRAngle(M_2_PI_EXCLUSIVE)); 
        QVERIFY(freeAngles[1].start == WRAngle(0));
        QVERIFY(freeAngles[1].end == WRAngle(M_PI / 2.0f));
    }

    {
        RadialSweep rs(observer, obstacles, obstacleRadius, interval, radius);
        QVector<AngleInterval> freeAngles = rs.getFreeAngles();
        QVector<AngleInterval> obstructedAngles = rs.getObstructedAngles();

        QVERIFY(freeAngles.size() == 2);
        QVERIFY(freeAngles[0].start == WRAngle(M_PI * 3.0f / 2.0f));
        QVERIFY(freeAngles[0].end == WRAngle(M_2_PI_EXCLUSIVE)); 
        QVERIFY(freeAngles[1].start == WRAngle(0));
        QVERIFY(freeAngles[1].end == WRAngle(M_PI / 2.0f));
    } 

    obstacles.push_back(Vec2(-1, 0));
}

void TestRadialSweep::testLargestInterval() {
    {
        QVector<AngleInterval> intervals = {
            { WRAngle(0), WRAngle(M_PI / 6.0f) },
            { WRAngle(M_PI), WRAngle(3.0f * M_PI / 2.0f) },
        };

        AngleInterval largest = RadialSweep::getLargestAngleInterval(intervals);

        QVERIFY(largest.start == WRAngle(M_PI));
        QVERIFY(largest.end == WRAngle(3.0f * M_PI / 2.0f));
    }

    {
        QVector<AngleInterval> intervals = {
            { WRAngle(0), WRAngle(M_PI) },
            { WRAngle(M_PI), WRAngle(M_2_PI_EXCLUSIVE) },
        };

        AngleInterval largest = RadialSweep::getLargestAngleInterval(intervals);

        QVERIFY(largest.start == WRAngle(0));
        QVERIFY(largest.end == WRAngle(M_2_PI_EXCLUSIVE));
    }
}

void TestRadialSweep::testCenterOfInterval() {
    AngleInterval interval(0, M_PI / 2.0f);
    WRAngle center = RadialSweep::getCenterOfInterval(interval);

    QVERIFY(center == WRAngle(M_PI / 4.0f));

    interval = { WRAngle(0), WRAngle(M_PI) };
    center = RadialSweep::getCenterOfInterval(interval);

    QVERIFY(center == WRAngle(M_PI / 2.0f));

    interval = { WRAngle(M_PI), WRAngle(M_PI / 2.0f) };
    center = RadialSweep::getCenterOfInterval(interval);

    QVERIFY(center == WRAngle(M_PI * 7.0f / 4.0f));
}

static TestRadialSweep TEST_RADIALSWEEP;
#include "test_radialsweep.moc"