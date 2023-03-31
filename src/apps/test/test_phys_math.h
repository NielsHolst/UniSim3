#ifndef TEST_PHYS_MATH_H
#define TEST_PHYS_MATH_H

#include "autotest.h"

class TestPhysMath : public QObject
{
    Q_OBJECT
private slots:
    void testCumNormal();
    void testInvCumNormal();
    void testInferNormal();
    void testInvNormalRange();

    void testSvp();
    void testSvpSlope();
};

DECLARE_TEST(TestPhysMath)

#endif
