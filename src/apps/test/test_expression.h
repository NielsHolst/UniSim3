#ifndef TEST_EXPRESSION_H
#define TEST_EXPRESSION_H

#include "autotest.h"

class TestExpression : public QObject
{
    Q_OBJECT
private slots:
    void testPush();
    void testToPostfix();
    void testEmpty();
    void testSingle();
    void testAddition();
    void testResultType();
    void testNegation();
    void testExponentiation();
    void testFunctionCall();
    void testParentheses();
};

DECLARE_TEST(TestExpression)

#endif
