#ifndef TEST_EXPRESSION_OPERATION_H
#define TEST_EXPRESSION_OPERATION_H

#include "autotest.h"

class TestOperate : public QObject
{
    Q_OBJECT
private slots:
    void testAdd();
    void testAddNumbers();
    void testAddDate();
    void testAddTime();
    void testAddDateTime();
    void testError();
    void testVector();
};

DECLARE_TEST(TestOperate)

#endif
