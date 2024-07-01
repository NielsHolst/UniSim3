/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <QDate>
#include <base/convert.h>
#include "exception_expectation.h"
#include "test_convert_date.h"

using namespace base;

void TestConvertDate::testFromString() {
    QDate date;
    bool excepted (false);

    date = convert<QDate>(QString("24/12/2021"));
    QCOMPARE(QDate(2021,12,24), date);

    date = convert<QDate>("24/12/2021");
    QCOMPARE(QDate(2021,12,24), date);

    date = convert<QDate>("2021/12/24");
    QCOMPARE(QDate(2021,12,24), date);

    date = convert<QDate>("/12/24/2021");
    QCOMPARE(QDate(2021,12,24), date);

    date = convert<QDate>("24-12-2021");
    QCOMPARE(QDate(2021,12,24), date);

    date = convert<QDate>("2021-12-24");
    QCOMPARE(QDate(2021,12,24), date);

    date = convert<QDate>("24.12.2021");
    QCOMPARE(QDate(2021,12,24), date);

    try {
        convert<QDate>("24/12/2021/2021");
    }
    EXPECTED_EXCEPTION;

    try {
        convert<QDate>("31/02/2021");
    }
    EXPECTED_EXCEPTION;
}
