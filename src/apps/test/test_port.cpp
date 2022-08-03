#include <iostream>
#include <limits>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QTimeZone>
#include <base/date_time.h>
#include <base/exception.h>
#include <base/port.h>
#include "test_port.h"

using std::exception;
using std::numeric_limits;
using namespace base;

#define MAX_NUMBER(X,Y) const X Y = numeric_limits<X>::max();
MAX_NUMBER(int, MAX_INT)
MAX_NUMBER(long int, MAX_LONG_INT)
MAX_NUMBER(long long int, MAX_LONG_LONG_INT)
MAX_NUMBER(float, MAX_FLOAT)
MAX_NUMBER(double, MAX_DOUBLE)
MAX_NUMBER(long double, MAX_LONG_DOUBLE)


void TestPort::initTestCase() {
    std::cout << numeric_limits<int>::max()  << " "
              << numeric_limits<long int>::max()<< " "
              << numeric_limits<long long int>::max()<< " "
              << numeric_limits<float>::max()<< " "
              << numeric_limits<double>::max()<< " "
              << numeric_limits<long double>::max()<< "\n";
    QVERIFY(MAX_INT == MAX_LONG_INT);
}

//
// Scalar
//

void TestPort::testFixedInt() {
    Port j;
    int i;
    j.data(&i);
    j.equals(5);
    QCOMPARE(i, 5);
    QCOMPARE(j.value<int>(), 5);
}

void TestPort::testPtrInt() {
    int i{5};
    Port j;
    j.data(&i);
    i = 6;
    QCOMPARE(j.value<int>(), 6);
}

void TestPort::testFixedString() {
    QString s;
    Port j;
    j.data(&s);
    j.equals(QString("aBc"));
    QCOMPARE(s, QString("aBc"));
    QCOMPARE(j.value<QString>(), QString("aBc"));
}

void TestPort::testPtrString() {
    QString s{};
    Port j;
    j.data(&s);
    s = "aBc6";
    QCOMPARE(j.value<QString>(), QString("aBc6"));
}

//
// Vector
//

void TestPort::testIntVector() {
    Port j;
    QVector<int> data, val;
    data.fill(0,3);
    j.data(&data);
    val << 7 << 9 << 13;
    j.equals(val);
    QCOMPARE(j.value<QVector<int>>(), val);
}

void TestPort::testStringVector() {
    Port j;
    QVector<QString> data, val;
    j.data(&data);
    val << "7" << "9" << "13";
    j.equals(val);
    QCOMPARE(j.value<QVector<QString>>(), val);
}

//
// Old-fashioned C-string
//

void TestPort::testFixedCString() {
    Port j;
    QString data;
    j.data(&data);
    j.equals("aBc");
    QCOMPARE(j.value<QString>(), QString("aBc"));
}

//
// US Date
//
void TestPort::testUSDate() {
    Port j;
    QDate data;
    j.data(&data);
    j.equals("/12/24/2015");
    QCOMPARE(j.value<QDate>(), QDate(2015,12,24));
    j.equals("/12/24/15");
    QCOMPARE(j.value<QDate>(), QDate(1915,12,24));
}


//
// Reset
//

void TestPort::testResetBool() {
    Port j;
    bool data(true);
    j.data(&data).zeroAtReset();
    QCOMPARE(j.value<bool>(), false);
    data = true;
    j.reset();
    QCOMPARE(j.value<bool>(), false);
}

void TestPort::testResetBoolVector() {
    typedef QVector<bool> BoolVector;
    Port j;
    BoolVector data{true, false, true},
               value{false, false, false};
    j.data(&data).zeroAtReset();
    QCOMPARE(j.value<BoolVector>(), data);
    j.reset();
    QCOMPARE(j.value<BoolVector>(), value);
}

void TestPort::testResetLongInt() {
    Port j;
    long int data;
    j.data(&data).zeroAtReset();
    QCOMPARE(j.value<long int>(), 0L);
    data = 7L;
    j.reset();
    QCOMPARE(j.value<long int>(), 0L);
}

void TestPort::testResetString() {
    Port j;
    QString data, empty;
    j.data(&data).zeroAtReset();
    QCOMPARE(j.value<QString>(), empty);
    data = "abc";
    j.reset();
    QCOMPARE(j.value<QString>(), empty);
}

//
// All conversions
//

#define CONV2(X,X2,X3,Y,Y2,Y3) \
    void TestPort::test##X##To##Y##ScalarToScalar() { \
        X2 x; \
        Y2 y2; \
        bool escaped{false}; \
        Port y; \
        y.data(&x); \
        x = X3; \
        try { \
            y2 = y.value<Y2>(); \
        } \
        catch (exception &) { \
            escaped = true; \
        } \
        QVERIFY(!escaped); \
        QCOMPARE(y2, Y3); \
    } \
    void TestPort::test##X##To##Y##VectorToScalar() { \
        QVector<X2> x; \
        Port y; \
        y.data(&x); \
        x << X3; \
        QCOMPARE(y.value<Y2>(), Y3); \
    } \
    void TestPort::test##X##To##Y##ScalarToVector() { \
        X2 x; \
        Port y; \
        y.data(&x); \
        x = X3; \
        QCOMPARE(y.value<QVector<Y2>>(), QVector<Y2>() << Y3); \
    } \
    void TestPort::test##X##To##Y##VectorToVector() { \
        QVector<X2> x; \
        Port y; \
        y.data(&x); \
        x << X3 << X3; \
        QCOMPARE(y.value<QVector<Y2>>(), QVector<Y2>() << Y3 << Y3); \
    }

#define CONV2_FAIL(X,X2,X3,Y,Y2,Z) \
    void TestPort::test##X##To##Y##ScalarToScalar##Z() { \
        bool escaped{false}; \
        X2 x; \
        Port y; \
        y.data(&x); \
        x = X3; \
        try { \
            y.value<Y2>(); \
        } \
        catch (Exception &) { \
            escaped = true; \
        } \
        QVERIFY(escaped); \
    } \
    void TestPort::test##X##To##Y##VectorToScalar##Z() { \
        bool escaped{false}; \
        QVector<X2> x; \
        Port y; \
        y.data(&x); \
        x << X3; \
        try { \
            y.value<Y2>(); \
        } \
        catch (Exception &) { \
            escaped = true; \
        } \
        QVERIFY(escaped); \
    } \
    void TestPort::test##X##To##Y##ScalarToVector##Z() { \
        bool escaped{false}; \
        X2 x; \
        Port y; \
        y.data(&x); \
        x = X3; \
        try { \
            y.value<QVector<Y2>>(); \
        } \
        catch (Exception &) { \
            escaped = true; \
        } \
        QVERIFY(escaped); \
    } \
    void TestPort::test##X##To##Y##VectorToVector##Z() { \
        bool escaped{false}; \
        QVector<X2> x; \
        Port y; \
        y.data(&x); \
        x << X3 << X3; \
        try { \
            y.value<QVector<Y2>>(); \
        } \
        catch (Exception &) { \
            escaped = true; \
        } \
        QVERIFY(escaped); \
    }

const bool BOOL = true;
const char CHAR = 'X';
const QDate DATE = QDate(2001,2,10);
const int TIME_HOURS = 11;
const QTime TIME = QTime(TIME_HOURS, 0);
const QDateTime DATE_TIME = makeDateTime(DATE, TIME);
const QDateTime DATE_TIME_MIDNIGHT = makeDateTime(DATE, QTime(0,0));

// Convert to bool
CONV2(Bool, bool, BOOL, Bool, bool, true)
CONV2(Char, char, CHAR, Bool, bool, true)
CONV2(Int, int, 7, Bool, bool, true)
CONV2(LongInt, long int, 7, Bool, bool, true)
CONV2(LongLongInt, long long int, 7, Bool, bool, true)
CONV2(Float, float, 7, Bool, bool, true)
CONV2(Double, double, 7, Bool, bool, true)
CONV2(LongDouble, long double, 7, Bool, bool, true)
CONV2_FAIL(Date, QDate, DATE, Bool, bool, )
CONV2_FAIL(Time, QTime, TIME, Bool, bool, )
CONV2_FAIL(DateTime, QDateTime, DATE_TIME, Bool, bool, )
CONV2(String, QString, "TRUE", Bool, bool, true)
CONV2_FAIL(String, QString, "X", Bool, bool, SyntaxError)
CONV2_FAIL(String, QString, "", Bool, bool, Empty)

// Convert to char
CONV2(Bool, bool, BOOL, Char, char, char(1))
CONV2(Char, char, CHAR, Char, char, CHAR)
CONV2(Int, int, 7, Char, char, char(7))
CONV2(LongInt, long int, 7, Char, char, char(7))
CONV2(LongLongInt, long long int, 7, Char, char, char(7))
CONV2(Float, float, 7, Char, char, char(7))
CONV2(Double, double, 7, Char, char, char(7))
CONV2(LongDouble, long double, 7, Char, char, char(7))
CONV2_FAIL(Date, QDate, DATE, Char, char, )
CONV2_FAIL(Time, QTime, TIME, Char, char, )
CONV2_FAIL(DateTime, QDateTime, DATE_TIME, Char, char, )
CONV2(String, QString, "X", Char, char, 'X')
CONV2_FAIL(String, QString, "Xa", Char, char, SyntaxError)
CONV2_FAIL(String, QString, "", Char, char, Empty)

// Convert to int
CONV2(Bool, bool, BOOL, Int, int, 1)
CONV2(Char, char, CHAR, Int, int, int(CHAR))
CONV2(Int, int, 7, Int, int, 7)
CONV2(LongInt, long int, 7, Int, int, 7)
CONV2(LongLongInt, long long int, 7, Int, int, 7)
CONV2(Float, float, 7, Int, int, 7)
CONV2(Double, double, 7, Int, int, 7)
CONV2(LongDouble, long double, 7, Int, int, 7)
CONV2(Date, QDate, DATE, Int, int, 41)
CONV2(Time, QTime, TIME, Int, int, TIME_HOURS)
CONV2(DateTime, QDateTime, DATE_TIME, Int, int, 41)
CONV2(String, QString, QString::number(7), Int, int, 7)
CONV2_FAIL(LongLongInt, long long int, MAX_LONG_LONG_INT, Int, int, Overflow)
CONV2_FAIL(Float, float, MAX_FLOAT, Int, int, Overflow)
CONV2_FAIL(Double, double, MAX_DOUBLE, Int, int, Overflow)
CONV2_FAIL(LongDouble, long double, MAX_LONG_DOUBLE, Int, int, Overflow)
CONV2_FAIL(String, QString, "7a", Int, int, SyntaxError)
CONV2_FAIL(String, QString, "", Int, int, Empty)

// Convert to long int
CONV2(Bool, bool, BOOL, LongInt, long int, 1L)
CONV2(Char, char, CHAR, LongInt, long int, long(CHAR))
CONV2(Int, int, 7, LongInt, long int, 7L)
CONV2(LongInt, long int, 7, LongInt, long int, 7L)
CONV2(LongLongInt, long long int, 7, LongInt, long int, 7L)
CONV2(Float, float, 7, LongInt, long int, 7L)
CONV2(Double, double, 7, LongInt, long int, 7L)
CONV2(LongDouble, long double, 7, LongInt, long int, 7L)
CONV2(Date, QDate, DATE, LongInt, long int, 41L)
CONV2(Time, QTime, TIME, LongInt, long int, TIME_HOURS*1L)
CONV2(DateTime, QDateTime, DATE_TIME, LongInt, long int, 41L)
CONV2(String, QString, QString::number(7), LongInt, long int, 7L)
CONV2_FAIL(LongLongInt, long long int, MAX_LONG_LONG_INT, LongInt, long int, Overflow)
CONV2_FAIL(Float, float, MAX_FLOAT, LongInt, long int, Overflow)
CONV2_FAIL(Double, double, MAX_DOUBLE, LongInt, long int, Overflow)
CONV2_FAIL(LongDouble, long double, MAX_LONG_DOUBLE, LongInt, long int, Overflow)
CONV2_FAIL(String, QString, "7a", LongInt, long int, SyntaxError)

// Convert to long long int
CONV2(Bool, bool, BOOL, LongLongInt, long long int, 1LL)
CONV2(Char, char, CHAR, LongLongInt, long long int, 0LL + long(CHAR))
CONV2(Int, int, 7, LongLongInt, long long int, 7LL)
CONV2(LongInt, long int, 7, LongLongInt, long long int, 7LL)
CONV2(LongLongInt, long long int, 7, LongLongInt, long long int, 7LL)
CONV2(Float, float, 7, LongLongInt, long long int, 7LL)
CONV2(Double, double, 7, LongLongInt, long long int, 7LL)
CONV2(LongDouble, long double, 7, LongLongInt, long long int, 7LL)
CONV2(Date, QDate, DATE, LongLongInt, long long int, 41LL)
CONV2(Time, QTime, TIME, LongLongInt, long long int, TIME_HOURS*1LL)
CONV2(DateTime, QDateTime, DATE_TIME, LongLongInt, long long int, 41LL)
CONV2(String, QString, QString::number(7), LongLongInt, long long int, 7LL)
CONV2_FAIL(Float, float, MAX_FLOAT, LongLongInt, long long int, Overflow)
CONV2_FAIL(Double, double, MAX_DOUBLE, LongLongInt, long long int, Overflow)
CONV2_FAIL(LongDouble, long double, MAX_LONG_DOUBLE, LongLongInt, long long int, Overflow)
CONV2_FAIL(String, QString, "7a", LongLongInt, long long int, SyntaxError)

// Convert to float
CONV2(Bool, bool, true, Float, float, 1.)
CONV2(Char, char, CHAR, Float, float, float(CHAR))
CONV2(Int, int, 3, Float, float, 3.)
CONV2(LongInt, long int, 3, Float, float, 3.)
CONV2(LongLongInt, long long int, 3, Float, float, 3.)
CONV2(Float, float, 7, Float, float, 7.)
CONV2(Double, double, 7, Float, float, 7.)
CONV2(LongDouble, long double, 7., Float, float, 7.)
CONV2(Date, QDate, DATE, Float, float, 41.)
CONV2(Time, QTime, TIME, Float, float, TIME_HOURS*1.)
CONV2(DateTime, QDateTime, DATE_TIME, Float, float, 41.4583320618) // actual numerical imprecision
CONV2(String, QString, QString::number(7), Float, float, 7.)
CONV2_FAIL(LongDouble, long double, MAX_LONG_DOUBLE, Float, float, Overflow)
CONV2_FAIL(String, QString, QString("7a"), Float, float, SyntaxError)

// Convert to double
CONV2(Bool, bool, true, Double, double, 1.)
CONV2(Char, char, CHAR, Double, double, double(CHAR))
CONV2(Int, int, 3, Double, double, 3.)
CONV2(LongInt, long int, 3, Double, double, 3.)
CONV2(LongLongInt, long long int, 3, Double, double, 3.)
CONV2(Float, float, 7, Double, double, 7.)
CONV2(Double, double, 7, Double, double, 7.)
CONV2(LongDouble, long double, 3.12, Double, double, 3.12)
CONV2(Date, QDate, DATE, Double, double, 41.)
CONV2(Time, QTime, TIME, Double, double, TIME_HOURS*1.)
CONV2(DateTime, QDateTime, DATE_TIME, Double, double, 41.+11./24.)
CONV2(String, QString, QString::number(7), Double, double, 7.)
CONV2_FAIL(LongDouble, long double, MAX_LONG_DOUBLE, Double, double, Overflow)
CONV2_FAIL(String, QString, QString("7a"), Double, double, SyntaxError)

// Convert to long double
CONV2(Bool, bool, true, LongDouble, long double, 1.0L)
CONV2(Char, char, CHAR, LongDouble, long double, 0.0L + double(CHAR))
CONV2(Int, int, 3, LongDouble, long double, 3.0L)
CONV2(LongInt, long int, 3, LongDouble, long double, 3.0L)
CONV2(LongLongInt, long long int, 3, LongDouble, long double, 3.0L)
CONV2(Float, float, 7, LongDouble, long double, 7.0L)
CONV2(Double, double, 7, LongDouble, long double, 7.0L)
CONV2(LongDouble, long double, 3.12L, LongDouble, long double, 3.12L)
CONV2(Date, QDate, DATE, LongDouble, long double, 41.0L)
CONV2(Time, QTime, TIME, LongDouble, long double, TIME_HOURS*1.0L)
CONV2(DateTime, QDateTime, DATE_TIME, LongDouble, long double, 41.0L+11.0L/24.0L)
CONV2(String, QString, QString::number(7), LongDouble, long double, 7.0L)
CONV2_FAIL(String, QString, QString("7a"), LongDouble, long double, SyntaxError)

// Convert to QDate
CONV2_FAIL(Bool, bool, BOOL, Date, QDate, )
CONV2_FAIL(Char, char, CHAR, Date, QDate, )
CONV2_FAIL(Int, int, 41, Date, QDate, )
CONV2_FAIL(LongInt, long int, 41, Date, QDate, )
CONV2_FAIL(LongLongInt, long long int, 41, Date, QDate, )
CONV2_FAIL(Float, float, 41, Date, QDate, )
CONV2_FAIL(Double, double, 41, Date, QDate, )
CONV2_FAIL(LongDouble, long double, 41, Date, QDate, )
CONV2(Date, QDate, DATE, Date, QDate, DATE)
CONV2_FAIL(Time, QTime, TIME, Date, QDate, )
CONV2(DateTime, QDateTime, DATE_TIME, Date, QDate, DATE)
CONV2(String, QString, "10/2/2001", Date, QDate, DATE)
CONV2_FAIL(String, QString, "13/13/2001", Date, QDate, Illegal)
CONV2_FAIL(String, QString, "", Date, QDate, Empty)

// Convert to QTime
CONV2_FAIL(Bool, bool, BOOL, Time, QTime, )
CONV2(Char, char, TIME_HOURS, Time, QTime, TIME)
CONV2(Int, int, TIME_HOURS, Time, QTime, TIME)
CONV2(LongInt, long int, TIME_HOURS, Time, QTime, TIME)
CONV2(LongLongInt, long long int, TIME_HOURS, Time, QTime, TIME)
CONV2(Float, float, TIME_HOURS, Time, QTime, TIME)
CONV2(Double, double, TIME_HOURS, Time, QTime, TIME)
CONV2(LongDouble, long double, TIME_HOURS, Time, QTime, TIME)
CONV2_FAIL(Date, QDate, DATE, Time, QTime, )
CONV2(Time, QTime, TIME, Time, QTime, TIME)
CONV2(DateTime, QDateTime, DATE_TIME, Time, QTime, TIME)
CONV2(String, QString, "11:00:00", Time, QTime, TIME)
CONV2_FAIL(String, QString, "11-00", Time, QTime, SyntaxError)
CONV2_FAIL(String, QString, "30:30", Time, QTime, Illegal)
CONV2_FAIL(String, QString, "", Time, QTime, Empty)

// Convert to QDateTime
CONV2_FAIL(Bool, bool, BOOL, DateTime, QDateTime, )
CONV2_FAIL(Char, char, CHAR, DateTime, QDateTime, )
CONV2_FAIL(Int, int, 41, DateTime, QDateTime, )
CONV2_FAIL(LongInt, long int, 41, DateTime, QDateTime, )
CONV2_FAIL(LongLongInt, long long int, 41, DateTime, QDateTime, )
CONV2_FAIL(Float, float, 41, DateTime, QDateTime, )
CONV2_FAIL(Double, double, 41, DateTime, QDateTime, )
CONV2_FAIL(LongDouble, long double, 41, DateTime, QDateTime, )
CONV2(Date, QDate, DATE, DateTime, QDateTime, DATE_TIME_MIDNIGHT)
CONV2_FAIL(Time, QTime, TIME, DateTime, QDateTime, )
CONV2(DateTime, QDateTime, DATE_TIME, DateTime, QDateTime, DATE_TIME)
CONV2(String, QString, "10/2/2001 11:00:00", DateTime, QDateTime, DATE_TIME)
CONV2_FAIL(String, QString, "13/13/2001", DateTime, QDateTime, Illegal)
CONV2_FAIL(String, QString, "", DateTime, QDateTime, Empty)
