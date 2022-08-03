#ifndef TEST_MINIMISE_H
#define TEST_MINIMISE_H

#include "autotest.h"

class TestMinimise : public QObject
{
    Q_OBJECT
private slots:
    void testBrent();
    void testWrapper();

    void testBrentWithArg();
    void testWrapperWithArg();
};

DECLARE_TEST(TestMinimise)

#endif
