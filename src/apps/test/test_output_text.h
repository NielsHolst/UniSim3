#ifndef TEST_OUTPUT_TEXT_H
#define TEST_OUTPUT_TEXT_H

#include "autotest.h"

class TestOutputText : public QObject
{
    Q_OBJECT
private slots:
    void testOk();
};

DECLARE_TEST(TestOutputText)

#endif
