#ifndef TESTSUITE_HH
#define TESTSUITE_HH

#include <QObject>
#include <QTest>

class TestSuite: public QObject
{
public:
     TestSuite();

     static QVector<QObject*> &suite();
};

#endif