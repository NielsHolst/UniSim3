#ifndef TEST_DATE_TIME_SIGNAL_H
#define TEST_DATE_TIME_SIGNAL_H

#include "autotest.h"

class TestDateTimeSignal : public QObject
{
    Q_OBJECT
private slots:
    void testBeforeDates();
    void testInsideDates();
    void testAfterDates();

    void testBeforeDatesWinter();
    void testInsideDatesWinter();
    void testAfterDatesWinter();

    void testBeforeTimes();
    void testInsideTimes();
    void testAfterTimes();

    void testBeforeFirstDateTime();
    void testInsideFirstDateTime();
    void testInsideLastDateTime();
    void testAfterLastDateTime();
};

DECLARE_TEST(TestDateTimeSignal)

#endif
