#ifndef TEST_BARE_DATE_H
#define TEST_BARE_DATE_H

#include "autotest.h"

class TestBareDate : public QObject
{
    Q_OBJECT
private slots:
    void testComparisonsWithBareDate();
    void testComparisonsWithQDate();
};

DECLARE_TEST(TestBareDate)

#endif
