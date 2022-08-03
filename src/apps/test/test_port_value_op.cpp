#include <QDate>
#include <base/port_value_op.h>
#include <base/exception.h>
#include "test_port_value_op.h"

using namespace base;

void TestPortValueOp::testAllocate() {
    void *x = port_value_op::allocate(PortType::Double);
    double *y = reinterpret_cast<double*>(x);
    *y = 12.3;
    QCOMPARE(*y, 12.3);
    port_value_op::deallocate(PortType::Double, x);
}

void TestPortValueOp::testAccumulate() {
    int a(12), b(23);
    port_value_op::accumulate(PortType::Int, &a, &b);
    QCOMPARE(a, 35);
    QCOMPARE(b, 23);

    double x(12.3), y(34.5);
    port_value_op::accumulate(PortType::Double, &x, &y);
    QCOMPARE(x, 46.8);
    QCOMPARE(y, 34.5);

    QDate u,v;
    bool excepted(false);
    try {
        port_value_op::accumulate(PortType::Date, &u, &v);
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestPortValueOp::testDivide() {
    int a(12);
    port_value_op::divide(PortType::Int, &a, 3);
    QCOMPARE(a, 4);

    double x(120);
    port_value_op::divide(PortType::Double, &x, 3);
    QCOMPARE(x, 40.);

    QDate u;
    bool excepted(false);
    try {
        port_value_op::divide(PortType::Date, &u, 3);
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestPortValueOp::testAdd() {
    int a(12), b(23), c;
    port_value_op::add(PortType::Int, &c, &a, &b);
    QCOMPARE(a, 12);
    QCOMPARE(b, 23);
    QCOMPARE(c, 35);

    double x(12.3), y(34.5), z;
    port_value_op::add(PortType::Double, &z, &x, &y);
    QCOMPARE(x, 12.3);
    QCOMPARE(y, 34.5);
    QCOMPARE(z, 46.8);

    QDate u,v,w;
    bool excepted(false);
    try {
        port_value_op::add(PortType::Date, &w, &u, &v);
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestPortValueOp::testMin() {
    int a(12), b(23), c;
    port_value_op::min(PortType::Int, &c, &a, &b);
    QCOMPARE(a, 12);
    QCOMPARE(b, 23);
    QCOMPARE(c, 12);

    double x(12.3), y(34.5), z;
    port_value_op::min(PortType::Double, &z, &x, &y);
    QCOMPARE(x, 12.3);
    QCOMPARE(y, 34.5);
    QCOMPARE(z, 12.3);

    QDate s(2001, 12, 24), t(2001, 12, 25), r;
    port_value_op::min(PortType::Date, &r, &s, &t);
    QCOMPARE(s, QDate(2001, 12, 24));
    QCOMPARE(t, QDate(2001, 12, 25));
    QCOMPARE(r, QDate(2001, 12, 24));

    QString u,v,w;
    bool excepted(false);
    try {
        port_value_op::min(PortType::String, &w, &u, &v);
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestPortValueOp::testMax() {
    int a(12), b(23), c;
    port_value_op::max(PortType::Int, &c, &a, &b);
    QCOMPARE(a, 12);
    QCOMPARE(b, 23);
    QCOMPARE(c, 23);

    double x(12.3), y(34.5), z;
    port_value_op::max(PortType::Double, &z, &x, &y);
    QCOMPARE(x, 12.3);
    QCOMPARE(y, 34.5);
    QCOMPARE(z, 34.5);

    QDate s(2001, 12, 24), t(2001, 12, 25), r;
    port_value_op::max(PortType::Date, &r, &s, &t);
    QCOMPARE(s, QDate(2001, 12, 24));
    QCOMPARE(t, QDate(2001, 12, 25));
    QCOMPARE(r, QDate(2001, 12, 25));

    QString u,v,w;
    bool excepted(false);
    try {
        port_value_op::max(PortType::String, &w, &u, &v);
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestPortValueOp::testLt() {
    int a(12), b(23);
    bool ok = port_value_op::lt(PortType::Int, &a, &b);
    QCOMPARE(a, 12);
    QCOMPARE(b, 23);
    QVERIFY(ok);
}

void TestPortValueOp::testGt() {
    int a(12), b(23);
    bool ok = port_value_op::gt(PortType::Int, &b, &a);
    QCOMPARE(a, 12);
    QCOMPARE(b, 23);
    QVERIFY(ok);
}

void TestPortValueOp::testEq() {
    int a(123), b(123);
    bool ok = port_value_op::eq(PortType::Int, &a, &b);
    QCOMPARE(a, 123);
    QCOMPARE(b, 123);
    QVERIFY(ok);
}

void TestPortValueOp::testLe() {
    int a(12), b(23);
    bool ok = port_value_op::le(PortType::Int, &a, &b);
    QCOMPARE(a, 12);
    QCOMPARE(b, 23);
    QVERIFY(ok);

    int c(123), d(123);
    ok = port_value_op::le(PortType::Int, &c, &d);
    QCOMPARE(c, 123);
    QCOMPARE(d, 123);
    QVERIFY(ok);
}

void TestPortValueOp::testGe() {
    int a(12), b(23);
    bool ok = port_value_op::ge(PortType::Int, &b, &a);
    QCOMPARE(a, 12);
    QCOMPARE(b, 23);
    QVERIFY(ok);

    int c(123), d(123);
    ok = port_value_op::ge(PortType::Int, &c, &d);
    QCOMPARE(c, 123);
    QCOMPARE(d, 123);
    QVERIFY(ok);
}
