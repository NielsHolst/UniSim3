#ifndef TEST_CONVERT_DATE_TIME_H
#define TEST_CONVERT_DATE_TIME_H

#include "autotest.h"

class TestConvertDateTime : public QObject
{
    Q_OBJECT
private slots:
    void testFromString();
    void testMidnight();
};

DECLARE_TEST(TestConvertDateTime)

#endif
