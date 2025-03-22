#include "utils/testsuite/testsuite.hh"
#include "algorithm/pathplanner/potentialfield/potentialfield.hh"

class TestPotentialField : public TestSuite {
    Q_OBJECT
public:
    using TestSuite::TestSuite;
private slots:
    void testCalculation();
};

void TestPotentialField::testCalculation() {
    QVERIFY(true); // Example assertion
}

static TestPotentialField TEST_POTENTIALFIELD;
#include "test_potentialfield.moc"