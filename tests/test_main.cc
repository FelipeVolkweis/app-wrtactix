#include "utils/testsuite/testsuite.hh"
#include <QtTest/QtTest>


int main(int argc, char *argv[]) {
    int status = 0;
    auto runTest = [&status, argc, argv](QObject* obj) {
        status |= QTest::qExec(obj, argc, argv);
    };

    auto &suite = TestSuite::suite();
    for (auto it = suite.begin(); it != suite.end(); ++it) {
        runTest(*it);
    }
}