#ifndef TEST_VECTORIZATION_H
#define TEST_VECTORIZATION_H

#include "autotest.h"

class TestVectorization : public QObject
{
    Q_OBJECT
private slots:
    void testDifferentTypes();
};

DECLARE_TEST(TestVectorization)

#endif
