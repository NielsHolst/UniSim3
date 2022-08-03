#include <base/any_year.h>
#include <base/convert.h>
#include <base/test_num.h>
#include "test_convert.h"

using namespace base;

void TestConvert::testStringListFromString() {
    QString s("(A 7 X)");
    QStringList result, expected;
    expected << "A" << "7" << "X";
    result = convert<QStringList>(s);
    QCOMPARE(result, expected);
}

void TestConvert::testIntFromStringList() {
    QStringList list;
    QVector<int> expected, result;
    list << "7" << "9" << "13";
    expected << 7 << 9 << 13;
    result = convert<int, QVector>(list);
    QCOMPARE(result, expected);
}


void TestConvert::testDate() {
    try {
        QCOMPARE(QDate(2012,12,24), convert<QDate>("24/12/2012"));
        QCOMPARE(QDate(2012,12,24), convert<QDate>("2012/12/24"));
        QCOMPARE(QDate(2012,12,24), convert<QDate>("/12/24/2012"));

        QCOMPARE(QDate(2012,12,24), convert<QDate>("24.12.2012"));
        QCOMPARE(QDate(2012,12,24), convert<QDate>("24-12-2012"));
        QCOMPARE(QDate(2012,12,24), convert<QDate>("2012.12.24"));
        QCOMPARE(QDate(2012,12,24), convert<QDate>("2012-12-24"));

        QVERIFY(equals(QDate(2012,12,24), convert<QDate>("24/12/*")));
        QVERIFY(equals(QDate(2012,12,24), convert<QDate>("*/12/24")));
        QVERIFY(equals(QDate(2012,12,24), convert<QDate>("/12/24/*")));

        QVERIFY(equals(QDate(2012,12,24), convert<QDate>("24.12.*")));
        QVERIFY(equals(QDate(2012,12,24), convert<QDate>("24-12-*")));
        QVERIFY(equals(QDate(2012,12,24), convert<QDate>("*.12.24")));
        QVERIFY(equals(QDate(2012,12,24), convert<QDate>("*-12-24")));
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Unexpected exception: " + ex.what()));
    }

    bool excepted{false};
    try {
        convert<QDate>("*/12/*");
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestConvert::testTimeToDouble() {
    QTime time(16,10,30);
    double hours;
    try {
        hours = convert<double>(time);
        QVERIFY(TestNum::eq(hours, 16.+10./60.+30./3600.));
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Unexpected exception: " + ex.what()));
    }
}

void TestConvert::testDoubleToTime() {
    double hours = 3. + 25./60. + 15./3600.;
    QTime time;
    try {
        time = convert<QTime>(hours);
        QVERIFY2(time == QTime(3,25,15), qPrintable(time.toString()));
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Unexpected exception: " + ex.what()));
    }
}

void TestConvert::testStringDoubleToTime() {
    double hours = 3. + 25./60. + 15./3600.;
    QTime time;
    try {
        time = convert<QTime>(QString::number(hours));
        QVERIFY2(time == QTime(3,25,15), qPrintable(time.toString()));
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Unexpected exception: " + ex.what()));
    }
}

