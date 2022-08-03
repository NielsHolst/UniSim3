#ifndef TEST_VECTOR_OP_H
#define TEST_VECTOR_OP_H

#include "autotest.h"

class TestVectorOp : public QObject
{
    Q_OBJECT
private slots:
    void testPlusVecVec();
    void testPlusVecScalar();
    void testPlusVecVecVec();
    void testPlusVecVecScalar();

    void testMinusVecVec();
    void testMinusVecScalar();
    void testMinusVecVecVec();
    void testMinusVecVecScalar();

    void testMultiplyVecVec();
    void testMultiplyVecScalar();
    void testMultiplyVecVecVec();
    void testMultiplyVecVecScalar();

    void testSum();
    void testWeightedAverage();
    void testSumOfProducts();
};

DECLARE_TEST(TestVectorOp)

#endif
