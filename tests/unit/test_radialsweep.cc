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
};

void TestRadialSweep::testCreateAngleEvents() {
    Vec2 observer(0, 0);
    QVector<Vec2> obstacles = { Vec2(1, 0) };
    AngleInterval interval(0, 2 * M_PI);
    float radius = 2.0;
    float obstacleRadius = 0.5;

    QVector<AngleEvent> events = RadialSweep::createEvents(obstacles, obstacleRadius, observer, radius);
    int last = events.size() - 1;

    QVERIFY(events.size() == 4);
    QVERIFY(events[0].angle == Angle(0));
    QVERIFY(events[0].start == true);
    QVERIFY(events[last].angle == Angle(M_2_PI_EXCLUSIVE));
    QVERIFY(events[last].start == false);
}

void TestRadialSweep::testMergeObstructedAngles() {
    AngleEvent e1 = { Angle(0.1f), true };
    AngleEvent e2 = { Angle(0.2f), true };
    AngleEvent e3 = { Angle(0.3f), false };
    AngleEvent e4 = { Angle(0.4f), false };

    QVector<AngleEvent> events = { e1, e2, e3, e4 };
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

    QVector<AngleEvent> events = RadialSweep::createEvents(obstacles, obstacleRadius, observer, radius);
    QVector<AngleInterval> intervals = RadialSweep::mergeObstructedIntervals(events);

    QVERIFY(events.size() > 0);
    QVERIFY(intervals.size() > 0);

    int n = intervals.size() - 1;
    QVERIFY(intervals[0].start == Angle(0));
    QVERIFY(intervals[0].end == Angle(2.0f * M_PI / 3.0f)); // 120 degrees
    QVERIFY(intervals[n].start == Angle(M_PI * 11.0f/6.0f)); // 330 degrees
    QVERIFY(intervals[n].end == Angle(M_2_PI_EXCLUSIVE));
}

void TestRadialSweep::testGetComplementIntervals() {
    QVector<AngleInterval> intervals = {
        { Angle(0), Angle(M_PI / 2.0f) },
        { Angle(M_PI), Angle(3.0f * M_PI / 2.0f) },
    };

    QVector<AngleInterval> complement = RadialSweep::getComplementIntervals(intervals);

    QVERIFY(complement.size() == 2);
    QVERIFY(complement[0].start == Angle(M_PI / 2.0f));
    QVERIFY(complement[0].end == Angle(M_PI));
    QVERIFY(complement[1].start == Angle(3.0f * M_PI / 2.0f));
    QVERIFY(complement[1].end == Angle(M_2_PI_EXCLUSIVE));

    intervals = {
        {Angle(0), Angle(M_PI * 1.0f / 6.0f)},
        {Angle(M_PI * 11.0f / 6.0f), Angle(M_2_PI_EXCLUSIVE)}
    };

    complement = RadialSweep::getComplementIntervals(intervals);

    QVERIFY(complement.size() == 1);
    QVERIFY(complement[0].start == Angle(M_PI * 1.0f / 6.0f));
    QVERIFY(complement[0].end == Angle(M_PI * 11.0f / 6.0f));
}

static TestRadialSweep TEST_RADIALSWEEP;
#include "test_radialsweep.moc"