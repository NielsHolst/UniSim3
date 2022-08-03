#ifndef TEST_PORT_VALUE_OP_H
#define TEST_PORT_VALUE_OP_H

#include "autotest.h"

class TestPortValueOp : public QObject
{
    Q_OBJECT
private slots:
    void testAllocate();
    void testAccumulate();
    void testDivide();
    void testAdd();
    void testMin();
    void testMax();
    void testLt();
    void testGt();
    void testEq();
    void testLe();
    void testGe();
};

DECLARE_TEST(TestPortValueOp)

#endif
