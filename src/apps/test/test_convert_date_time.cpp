#include <QDateTime>
#include <base/convert.h>
#include "exception_expectation.h"
#include "test_convert_date_time.h"

using namespace base;

void TestConvertDateTime::testFromString() {
    QDateTime dt;
    bool excepted (false);

    try {
        dt = convert<QDateTime>(QString("24/12/2021T12:30:45"));
        QCOMPARE(QDateTime(QDate(2021,12,24), QTime(12,30,45), Qt::UTC), dt);

        dt = convert<QDateTime>(QString("2021/12/24T12:30:45"));
        QCOMPARE(QDateTime(QDate(2021,12,24), QTime(12,30,45), Qt::UTC), dt);

        dt = convert<QDateTime>(QString("/12/24/2021T12:30:45"));
        QCOMPARE(QDateTime(QDate(2021,12,24), QTime(12,30,45), Qt::UTC), dt);
    }
    UNEXPECTED_EXCEPTION;

    try {
        convert<QDateTime>("24/12/2021/2021T12:30:45");
    }
    EXPECTED_EXCEPTION;

    try {
        convert<QDateTime>("31/02/2021/2021T12:30:45");
    }
    EXPECTED_EXCEPTION;

    try {
        convert<QDateTime>("24/12/2021/2021T12:60");
    }
    EXPECTED_EXCEPTION;
}

void TestConvertDateTime::testMidnight() {
    bool excepted (false);
    QDate date;
    QDateTime dt;

    try {
        date = convert<QDate>("24/12/2021");
        dt = convert<QDateTime>(date);
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(dt.date(), QDate(2021,12,24));
    QCOMPARE(dt.time(), QTime(0,0,0));

    try {
        dt = convert<QDateTime>("24/12/2021T00:00:00");
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(dt.date(), QDate(2021,12,24));
    QCOMPARE(dt.time(), QTime(0,0,0));

    try {
        dt = convert<QDateTime>("24/12/2021T24:00");
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(dt.date(), QDate(2021,12,25));
    QCOMPARE(dt.time(), QTime(0,0,0));

    try {
        dt = convert<QDateTime>("24/12/2021T55:00");
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(dt.date(), QDate(2021,12,26));
    QCOMPARE(dt.time(), QTime(7,0,0));
}

