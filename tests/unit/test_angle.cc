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
    WRAngle angle;
    QVERIFY(angle.radians() == 0.0f);
}

void TestAngle::testNormalization() {
    WRAngle angle1(2.0f * M_PI); // 360 degrees
    WRAngle angle0(0.0f); // 0 degrees
    QVERIFY(angle1 == angle0);

    WRAngle angle2(-M_PI); // -180 degrees
    WRAngle angle2_(M_PI);
    QVERIFY(angle2 == angle2_);

    WRAngle angle3(3.0f * M_PI); // 540 degrees
    QVERIFY(angle3 == angle2_);
}

void TestAngle::testArithmeticOperations() {
    WRAngle angle1(M_PI_2); // 90 degrees
    WRAngle angle2(M_PI);   // 180 degrees

    WRAngle sum = angle1 + angle2;
    WRAngle sum_(3.0f * M_PI_2);
    QVERIFY(sum == sum_);

    WRAngle difference = angle2 - angle1;
    WRAngle difference_(M_PI_2);
    QVERIFY(difference == difference_);

    WRAngle scaled = angle1 * 2.0f;
    WRAngle scaled_(M_PI);
    QVERIFY(scaled == scaled_);

    WRAngle divided = angle2 / 2.0f;
    WRAngle divided_(M_PI_2);
    QVERIFY(divided == divided_);
}

void TestAngle::testComparisonOperators() {
    WRAngle angle1(M_PI);
    WRAngle angle2(M_PI);
    WRAngle angle3(M_PI_2);

    QVERIFY(angle1 == angle2);
    QVERIFY(angle1 != angle3);
    QVERIFY(angle3 < angle1);
    QVERIFY(angle1 > angle3);
}

static TestAngle TEST_ANGLE;
#include "test_angle.moc"