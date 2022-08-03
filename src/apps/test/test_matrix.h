#ifndef TEST_MATRIX_H
#define TEST_MATRIX_H

#include "autotest.h"

class TestMatrix : public QObject
{
    Q_OBJECT
private slots:
    void testInt();
    void testString();
    void testResize();
};

DECLARE_TEST(TestMatrix)

#endif
