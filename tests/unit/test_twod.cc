#include "utils/testsuite/testsuite.hh"
#include "algorithm/geometry/twod/twod.hh"

#include <iostream>

class TestTwoD : public TestSuite {
    Q_OBJECT
public:
    using TestSuite::TestSuite;

private slots:
    void testIsPointOnSegment();
    void testInfiniteLineIntersection();
    void testBoundedLineIntersectionLineLine();
    void testBoundedLineIntersectionLineCircle();
};

static TestTwoD TEST_TWOD;

void TestTwoD::testIsPointOnSegment() {
    Vec2 p1(0, 0);
    Vec2 p2(4, 4);

    // Point lies on the segment
    Vec2 pointOnSegment(2, 2);
    QVERIFY(TwoD::isPointOnSegment(pointOnSegment, p1, p2));

    // Point lies outside the segment
    Vec2 pointOutsideSegment(5, 5);
    QVERIFY(!TwoD::isPointOnSegment(pointOutsideSegment, p1, p2));

    // Point coincides with an endpoint
    Vec2 pointAtEndpoint(4, 4);
    QVERIFY(TwoD::isPointOnSegment(pointAtEndpoint, p1, p2));

    // Point is collinear but outside the segment
    Vec2 collinearOutside(-1, -1);
    QVERIFY(!TwoD::isPointOnSegment(collinearOutside, p1, p2));
}

void TestTwoD::testInfiniteLineIntersection() {
    TwoD::Line line1(Vec2(0, 0), Vec2(4, 4));
    TwoD::Line line2(Vec2(0, 4), Vec2(4, 0));

    // Intersection of two non-parallel lines
    Vec2 intersection = TwoD::infiniteLineIntersection(line1, line2);
    QCOMPARE(intersection, Vec2(2, 2));

    // Parallel lines (no intersection)
    TwoD::Line line3(Vec2(0, 0), Vec2(4, 0));
    TwoD::Line line4(Vec2(0, 1), Vec2(4, 1));
    QCOMPARE(TwoD::infiniteLineIntersection(line3, line4), NULL_VEC);

    // Coincident lines (infinite intersections)
    TwoD::Line line5(Vec2(0, 0), Vec2(4, 4));
    TwoD::Line line6(Vec2(1, 1), Vec2(3, 3));
    QCOMPARE(TwoD::infiniteLineIntersection(line5, line6), NULL_VEC);
}

void TestTwoD::testBoundedLineIntersectionLineLine() {
    TwoD::Line line1(Vec2(0, 0), Vec2(4, 4));
    TwoD::Line line2(Vec2(0, 4), Vec2(4, 0));

    // Intersection of two bounded segments
    Vec2 intersection = TwoD::boundedLineIntersection(line1, line2);
    QCOMPARE(intersection, Vec2(2, 2));

    // Segments do not intersect
    TwoD::Line line3(Vec2(0, 0), Vec2(2, 2));
    TwoD::Line line4(Vec2(3, 3), Vec2(5, 5));
    QCOMPARE(TwoD::boundedLineIntersection(line3, line4), NULL_VEC);

    // Segments share an endpoint
    TwoD::Line line5(Vec2(0, 0), Vec2(2, 2));
    TwoD::Line line6(Vec2(2, 2), Vec2(4, 0));
    QCOMPARE(TwoD::boundedLineIntersection(line5, line6), Vec2(2, 2));
}

void TestTwoD::testBoundedLineIntersectionLineCircle() {
    Vec2 center(2, 2);
    float radius = 1.0;

    // Line intersects the circle at two points
    TwoD::Line line1(Vec2(0, 2), Vec2(4, 2));
    Vec2 intersection1 = TwoD::boundedLineIntersection(line1, center, radius);
    QVERIFY(intersection1 != NULL_VEC);

    // Line is tangent to the circle (one intersection point)
    TwoD::Line line2(Vec2(0, 3), Vec2(4, 3));
    Vec2 intersection2 = TwoD::boundedLineIntersection(line2, center, radius);
    QVERIFY(intersection2 != NULL_VEC);

    // Line does not intersect the circle
    TwoD::Line line3(Vec2(0, 4), Vec2(4, 4));
    Vec2 intersection3 = TwoD::boundedLineIntersection(line3, center, radius);
    QCOMPARE(intersection3, NULL_VEC);

    // Line segment is entirely inside the circle
    TwoD::Line line4(Vec2(2, 2), Vec2(2.5, 2.5));
    Vec2 intersection4 = TwoD::boundedLineIntersection(line4, center, radius);
    QVERIFY(intersection4 == NULL_VEC);
}

#include "test_twod.moc"