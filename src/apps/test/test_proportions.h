#ifndef TEST_PROPORTIONS_H
#define TEST_PROPORTIONS_H

#include "autotest.h"

class TestProportions : public QObject
{
    Q_OBJECT
private slots:
    void test1();
    void test2();
    void test3();
};

DECLARE_TEST(TestProportions)

#endif
