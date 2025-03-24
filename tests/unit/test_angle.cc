#include "utils/testsuite/testsuite.hh"
#include "types/angle.hh"

#include <iostream>

class TestAngle : public TestSuite {
    Q_OBJECT
public:
    using TestSuite::TestSuite;

private slots:
    void testDefaultConstructor();
    void testNormalization();
    void testArithmeticOperations();
    void testComparisonOperators();
};

void TestAngle::testDefaultConstructor() {
    Angle angle;
    QVERIFY(angle.radians() == 0.0f);
}

void TestAngle::testNormalization() {
    Angle angle1(2.0f * M_PI); // 360 degrees
    Angle angle0(0.0f); // 0 degrees
    QVERIFY(angle1 == angle0);

    Angle angle2(-M_PI); // -180 degrees
    Angle angle2_(M_PI);
    QVERIFY(angle2 == angle2_);

    Angle angle3(3.0f * M_PI); // 540 degrees
    QVERIFY(angle3 == angle2_);
}

void TestAngle::testArithmeticOperations() {
    Angle angle1(M_PI_2); // 90 degrees
    Angle angle2(M_PI);   // 180 degrees

    Angle sum = angle1 + angle2;
    Angle sum_(3.0f * M_PI_2);
    QVERIFY(sum == sum_);

    Angle difference = angle2 - angle1;
    Angle difference_(M_PI_2);
    QVERIFY(difference == difference_);

    Angle scaled = angle1 * 2.0f;
    Angle scaled_(M_PI);
    QVERIFY(scaled == scaled_);

    Angle divided = angle2 / 2.0f;
    Angle divided_(M_PI_2);
    QVERIFY(divided == divided_);
}

void TestAngle::testComparisonOperators() {
    Angle angle1(M_PI);
    Angle angle2(M_PI);
    Angle angle3(M_PI_2);

    QVERIFY(angle1 == angle2);
    QVERIFY(angle1 != angle3);
    QVERIFY(angle3 < angle1);
    QVERIFY(angle1 > angle3);
}

static TestAngle TEST_ANGLE;
#include "test_angle.moc"