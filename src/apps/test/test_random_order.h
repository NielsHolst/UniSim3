#ifndef TEST_RANDOM_ORDER_H
#define TEST_RANDOM_ORDER_H

#include "autotest.h"

class TestRandomOrder : public QObject
{
    Q_OBJECT
private slots:
    void test3();
};

DECLARE_TEST(TestRandomOrder)

#endif
