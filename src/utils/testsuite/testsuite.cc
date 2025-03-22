#include "testsuite.hh"

TestSuite::TestSuite() {
    suite().push_back(this);
}

QVector<QObject*> & TestSuite::suite() {
    static QVector<QObject*> objects;
    return objects;
}