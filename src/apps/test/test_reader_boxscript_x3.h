#ifndef TEST_READER_BOXSCRIPT_X3_H
#define TEST_READER_BOXSCRIPT_X3_H
#include "autotest.h"

class TestReaderBoxScriptX3 : public QObject
{
    Q_OBJECT
private slots:
    void testAuxAllTypes();
    void testConstructAmend();
    void testInitialize();
    void testInitializePortMissing();
    void testMultipleMatches();
    void testCondition();
    void testParentheses();
    void testDoubleError();
    void testOnOffByDate();
    void testMissingReference();
    void testMissingPlotTypes();
};

DECLARE_TEST(TestReaderBoxScriptX3)

#endif
