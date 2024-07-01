/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <QDate>
#include <base/convert.h>
#include "exception_expectation.h"
#include "test_convert_time.h"

using namespace base;

void TestConvertTime::testFromString() {
    QTime time;
    bool excepted (false);

    time = convert<QTime>(QString("12:30"));
    QCOMPARE(QTime(12,30), time);

    time = convert<QTime>(QString("12:30:45"));
    QCOMPARE(QTime(12,30,45), time);

    try {
        time = convert<QTime>(QString("24:00:00"));
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(QTime(0,0,0), time);

    try {
        time = convert<QTime>(QString("55:00:00"));
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(QTime(7,0,0), time);

    try {
        convert<QDate>("12");
    }
    EXPECTED_EXCEPTION;

    try {
        convert<QDate>("12:30:45:50");
    }
    EXPECTED_EXCEPTION;

    try {
        convert<QDate>("12:70");
    }
    EXPECTED_EXCEPTION;
}
