#include <base/test_num.h>
#include <base/vector_op.h>
#include "test_vector_op.h"

// Plus

void TestVectorOp::testPlusVecVec() {
    QVector<double> v, x, expected;
    v << 2 << 4 << 8;
    x << 5 << 13 << 11;
    expected << 7 << 17 << 19;
    vector_op::add(v,x);
    QCOMPARE(v, expected);
}

void TestVectorOp::testPlusVecScalar() {
    QVector<double> v, expected;
    double x = 13;
    v << 2 << 4 << 8;
    expected << 15 << 17 << 21;
    vector_op::add(v,x);
    QCOMPARE(v, expected);
}

void TestVectorOp::testPlusVecVecVec() {
    QVector<double> v, x, y, expected;
    x << 2 << 4 << 8;
    y << 5 << 13 << 11;
    expected << 7 << 17 << 19;
    vector_op::sum(v,x,y);
    QCOMPARE(v, expected);

}

void TestVectorOp::testPlusVecVecScalar() {
    QVector<double> v, x, expected;
    double y = 13;
    x << 2 << 4 << 8;
    expected << 15 << 17 << 21;
    vector_op::sum(v,x,y);
    QCOMPARE(v, expected);
}

// Minus

void TestVectorOp::testMinusVecVec() {
    QVector<double> v, x, expected;
    v << 2 << 4 << 8;
    x << 5 << 13 << 11;
    expected << -3 << -9 << -3;
    vector_op::subtract(v,x);
    QCOMPARE(v, expected);
}

void TestVectorOp::testMinusVecScalar() {
    QVector<double> v, expected;
    double x = 13;
    v << 2 << 4 << 8;
    expected << -11 << -9 << -5;
    vector_op::subtract(v,x);
    QCOMPARE(v, expected);
}

void TestVectorOp::testMinusVecVecVec() {
    QVector<double> v, x, y, expected;
    x << 2 << 4 << 8;
    y << 5 << 13 << 11;
    expected << -3 << -9 << -3;
    vector_op::difference(v,x,y);
    QCOMPARE(v, expected);

}

void TestVectorOp::testMinusVecVecScalar() {
    QVector<double> v, x, expected;
    double y = 13;
    x << 2 << 4 << 8;
    expected << -11 << -9 << -5;
    vector_op::difference(v,x,y);
    QCOMPARE(v, expected);
}

// Multiply

void TestVectorOp::testMultiplyVecVec() {
    QVector<double> v, x, expected;
    v << 2 << 4 << 8;
    x << 5 << 13 << 11;
    expected << 10 << 52 << 88;
    vector_op::multiply(v,x);
    QCOMPARE(v, expected);
}

void TestVectorOp::testMultiplyVecScalar() {
    QVector<double> v, expected;
    double x = 13;
    v << 2 << 4 << 8;
    expected << 26 << 52 << 104;
    vector_op::multiply(v,x);
    QCOMPARE(v, expected);
}

void TestVectorOp::testMultiplyVecVecVec() {
    QVector<double> v, x, y, expected;
    x << 2 << 4 << 8;
    y << 5 << 13 << 11;
    expected << 10 << 52 << 88;
    vector_op::product(v,x,y);
    QCOMPARE(v, expected);

}

void TestVectorOp::testMultiplyVecVecScalar() {
    QVector<double> v, x, expected;
    double y = 13;
    x << 2 << 4 << 8;
    expected << 26 << 52 << 104;
    vector_op::product(v,x,y);
    QCOMPARE(v, expected);
}

// Sum

void TestVectorOp::testSum() {
    QVector<double> x;
    double v, expected(29);
    x << 5 << 13 << 11;
    v = vector_op::sum(x);
    QCOMPARE(v, expected);

}

void TestVectorOp::testWeightedAverage() {
    QVector<double> x, y;
    double v, expected((2*7 + 4*11 + 8*13)/double(7+11+13));
    x << 2 << 4 << 8;
    y << 7 << 11 << 13;
    v = vector_op::weightedAverage(x,y);
    QVERIFY(TestNum::eq(v, expected));
}

void TestVectorOp::testSumOfProducts() {
    QVector<double> x, y;
    double v, expected(2*7 + 4*11 + 8*13);
    x << 2 << 4 << 8;
    y  << 7 << 11 << 13;
    v = vector_op::sumOfProducts(x,y);
    QVERIFY(TestNum::eq(v, expected));
}

