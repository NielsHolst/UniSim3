#ifndef TEST_CONVERT_DATE_H
#define TEST_CONVERT_DATE_H

#include "autotest.h"

class TestConvertDate : public QObject
{
    Q_OBJECT
private slots:
    void testFromString();
};

DECLARE_TEST(TestConvertDate)

#endif
