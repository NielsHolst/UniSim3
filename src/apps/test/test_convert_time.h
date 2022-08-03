#ifndef TEST_CONVERT_TIME_H
#define TEST_CONVERT_TIME_H

#include "autotest.h"

class TestConvertTime : public QObject
{
    Q_OBJECT
private slots:
    void testFromString();
};

DECLARE_TEST(TestConvertTime)

#endif
