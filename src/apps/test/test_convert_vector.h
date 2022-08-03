#ifndef TEST_CONVERT_VECTOR_H
#define TEST_CONVERT_VECTOR_H

#include "autotest.h"

class TestConvertVector : public QObject
{
    Q_OBJECT
private slots:
    void testIsInstance();
    void testBool();
    void testScalarToVector();
    void testVectorToScalar();
};

DECLARE_TEST(TestConvertVector)

#endif
