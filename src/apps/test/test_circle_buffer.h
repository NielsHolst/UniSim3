#ifndef TEST_CIRCLE_BUFFER_H
#define TEST_CIRCLE_BUFFER_H

#include "autotest.h"

class TestCircleBuffer : public QObject
{
    Q_OBJECT
private slots:
    void testStdVersion();
    void testUniSimVersion();
};

DECLARE_TEST(TestCircleBuffer)

#endif
