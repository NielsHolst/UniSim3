#ifndef TEST_OUTPUT_BUFFER_H
#define TEST_OUTPUT_BUFFER_H

#include "autotest.h"

class TestOutputBuffer : public QObject
{
    Q_OBJECT
private slots:
    void testOk();
};

DECLARE_TEST(TestOutputBuffer)

#endif
