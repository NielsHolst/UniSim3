#ifndef TEST_VECTOR_SUM_H
#define TEST_VECTOR_SUM_H

#include "autotest.h"

class TestVectorSum : public QObject
{
    Q_OBJECT
private slots:
    void testOk();
};

DECLARE_TEST(TestVectorSum)

#endif
