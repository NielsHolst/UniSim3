#ifndef TEST_RECORDS_H
#define TEST_RECORDS_H

#include "autotest.h"

class TestRecords : public QObject
{
    Q_OBJECT
private slots:
    void testDate();
    void testHour();
};

DECLARE_TEST(TestRecords)

#endif
