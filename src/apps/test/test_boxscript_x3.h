#ifndef TEST_BOX_SCRIPT_X3_H
#define TEST_BOX_SCRIPT_X3_H
#include "autotest.h"

class TestBoxScriptX3 : public QObject
{
    Q_OBJECT
private slots:
    void testNumberDef();
    void testNameDef();
    void testJokerDef();
    void testPortDef();
    void testComments();
    void testDate();
    void testBareDate();
    void testBareDateTime();
    void testBool();
    void testReference();
    void testDotsError();
    void testQuotedString();
    void testFunctionCall();
    void testBox();
    void testOperator();
    void testTooMuch();
    void testSyntaxError();
    void testNumber();
    void testAuxAllTypes();
    void testIfElse();
};

DECLARE_TEST(TestBoxScriptX3)

#endif
