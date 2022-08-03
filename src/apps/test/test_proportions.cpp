#include <iostream>
#include <base/proportions.h>
#include "test_proportions.h"

using namespace base;

void TestProportions::test1() {
    Proportions prop = new Proportions(this);
    double p=0.2;
    prop.add(p);
    QVector<double>
        P = prop.reconciled(),
        expected;
    expected << 0.2;
    QCOMPARE(P.size(), expected.size());
    QCOMPARE(P[0], expected[0]);
}

void TestProportions::test2() {
    Proportions prop = new Proportions(this);
    double p=0.2, q=0.6;
    prop.add(p);
    prop.add(q);
    QVector<double>
        P = prop.reconciled(),
        expected;
    expected << 0.14 << 0.54;
    QCOMPARE(P.size(), expected.size());
    QCOMPARE(P[0], expected[0]);
    QCOMPARE(P[1], expected[1]);
}

void TestProportions::test3() {
    Proportions prop = new Proportions(this);
    double p=0.2, q=0.6, r=0.5;
    prop.add(p);
    prop.add(q);
    prop.add(r);
    QVector<double>
        P = prop.reconciled(),
        expected;
    expected << 0.11 << 0.41 << 0.32;
    QCOMPARE(P.size(), expected.size());
    QCOMPARE(P[0], expected[0]);
    QCOMPARE(P[1], expected[1]);
    QCOMPARE(P[2], expected[2]);
}
