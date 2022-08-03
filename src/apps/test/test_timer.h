#ifndef TEST_TIMER_H
#define TEST_TIMER_H
#include "autotest.h"

class TestTimer : public QObject
{
    Q_OBJECT
private slots:
    void testDuration();
};

DECLARE_TEST(TestTimer)

#endif
