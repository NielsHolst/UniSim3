#include <base/port.h>
#include <base/vectorize.h>
#include "test_vectorization.h"

using namespace base;

void TestVectorization::testDifferentTypes() {
    QVector<Port*> source;
    QVector<int> dest, expected;
    const void *destResult;
    Port a("A"), b("B"), c("C");
    int i, j;
    double x;
    a.data(&i);
    b.data(&j);
    c.data(&x);
    source << &a << &b << &c;

    destResult = vectorize(Int, source);
    expected.fill(0, 3);
    dest = *reinterpret_cast<const QVector<int>*>(destResult);
    QCOMPARE(dest, expected);

    i = 8; j = 12;
    x = 100.2;
    destResult = vectorize(Int, source);
    dest = *reinterpret_cast<const QVector<int>*>(destResult);
    expected.clear();
    expected << 8 << 12 << 100;
    QCOMPARE(dest, expected);
}
