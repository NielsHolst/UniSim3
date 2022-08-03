#include <iostream>
#include <QVector>
#include <base/assign.h>
#include <base/date_time.h>
#include <base/port_transform.h>
#include "test_assign.h"

using namespace base;

void TestAssign::testToCharFromBool() {
    char dest(0);
    bool source(true);
    assign(Char, &dest, Bool, &source, PortTransform::Identity, 0);
    QCOMPARE(dest, char(1));
}

void TestAssign::testToCharFromChar() {
    char dest(0), source('K');
    assign(Char, &dest, Char, &source, PortTransform::Identity, 0);
    QCOMPARE(dest, source);
}

void TestAssign::testToCharFromBoolVector() {
    char dest(0);
    QVector<bool> source{true, false, true};
    assign(Char, &dest, BoolVector, &source, Any, 0);
    QCOMPARE(dest, char(1));
    assign(Char, &dest, BoolVector, &source, All, 0);
    QCOMPARE(dest, char(0));
}

void TestAssign::testToCharVectorFromBoolVector() {
    QVector<char> dest{0,0,0}, expected{1,0,1};
    QVector<bool> source{true, false, true};
    assign(CharVector, &dest, BoolVector, &source, PortTransform::Identity, 0);
    QCOMPARE(dest, expected);
}

void TestAssign::testToCharVectorFromCharVector() {
    QVector<char> dest{0,0,0}, source{'y','o','h'};
    assign(CharVector, &dest, CharVector, &source, PortTransform::Identity, 0);
    QCOMPARE(dest, source);
}

void TestAssign::testToCharVectorFromBool() {
    QVector<char> dest{0}, expected{1};
    bool source{true};
    assign(CharVector, &dest, Bool, &source, PortTransform::Identity, 0);
    QCOMPARE(dest, expected);
}

void TestAssign::testToStringFromInt() {
    QString dest;
    int source{7};
    assign(String, &dest, Int, &source, PortTransform::Identity, 0);
    QCOMPARE(dest, QString("7"));
}

void TestAssign::testToStringFromIntVector() {
    QString dest;
    QVector<int> source{7, 14, 15};
    assign(String, &dest, IntVector, &source, Sum, 0);
    QCOMPARE(dest, QString("36"));
}

void TestAssign::testToStringFromDate() {
    QString dest;
    QDate source(2015, 12, 24);
    assign(String, &dest, Date, &source, PortTransform::Identity, 0);
    QCOMPARE(dest, QString("2015/12/24"));
}

void TestAssign::testToIntFromDate() {
    int dest;
    QDate source(2015, 2, 10);
    assign(Int, &dest, Date, &source, PortTransform::Identity, 0);
    QCOMPARE(dest, 41);
}

void TestAssign::testToIntFromTime() {
    int dest;
    QTime source(10, 15, 0);
    assign(Int, &dest, Time, &source, PortTransform::Identity, 0);
    QCOMPARE(dest, 10);
}

void TestAssign::testToDoubleFromTime() {
    double dest;
    QTime source(10, 15, 0);
    assign(Double, &dest, Time, &source, PortTransform::Identity, 0);
    QCOMPARE(dest, 10. + 15./60.);
}

void TestAssign::testToDoubleFromDateTime() {
    double dest;
    QDateTime source = makeDateTime(QDate(2015, 2, 10), QTime(6,0,0));
    assign(Double, &dest, DateTime, &source, PortTransform::Identity, 0);
    QCOMPARE(dest, 41. + 6./24.);
}

void TestAssign::testToIntFromIntVector() {
    int dest;
    QVector<int> source;
    source << 7 << 11 << 13;
    assign(Int, &dest, IntVector, &source, PortTransform::Sum, 0);
    QCOMPARE(dest, 31);
}

void TestAssign::testToDoubleFromIntVector() {
    double dest;
    QVector<int> source;
    source << 7 << 11 << 13;
    assign(Double, &dest, IntVector, &source, PortTransform::Sum, 0);
    QCOMPARE(dest, 31.);
}

void TestAssign::testToIntFromStringVector() {
    bool excepted{false};
    int dest;
    QVector<QString> source;
    source << "7" << "11" << "13";
    try {
        assign(Int, &dest, StringVector, &source, PortTransform::Sum, 0);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestAssign::testToStringIdentityFromStringVector() {
    bool excepted{false};
    QString dest;
    QVector<QString> source;
    source << "ABZ";
    try {
        assign(String, &dest, StringVector, &source, PortTransform::Identity, 0);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(!excepted);
    QCOMPARE(dest, QString("ABZ"));
}

void TestAssign::testToStringVectorIdentityFromString() {
    bool excepted{false};
    QVector<QString> dest;
    QString source{"ABZ"};
    try {
        assign(StringVector, &dest, String, &source, PortTransform::Identity, 0);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(!excepted);
    QCOMPARE(dest, QVector<QString>() << "ABZ");
}
