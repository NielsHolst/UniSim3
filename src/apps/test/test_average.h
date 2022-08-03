#ifndef TEST_AVERAGE_H
#define TEST_AVERAGE_H
#include "autotest.h"

class TestAverage : public QObject
{
    Q_OBJECT
private slots:
    void testPeriod1();
    void testPeriodN();
};

DECLARE_TEST(TestAverage)

#endif
