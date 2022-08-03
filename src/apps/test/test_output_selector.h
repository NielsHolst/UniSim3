#ifndef TEST_OUTPUT_SELECTOR_H
#define TEST_OUTPUT_SELECTOR_H
#include "autotest.h"

class TestOutputSelector : public QObject
{
    Q_OBJECT
private slots:
    void testPeriod1Iter();
    void testPeriodNIter();
    void testFinal1Iter();
    void testFinalNIter();
    void testPeriod1IterSkip();
    void testPeriodNIterSkip();
};

DECLARE_TEST(TestOutputSelector)

#endif
