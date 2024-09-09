/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <iostream>
#include <QTimeZone>
#include <base/solar_math.h>
#include <base/test_num.h>
#include "test_solar_math.h"

using namespace base;
using namespace phys_math;
using namespace TestNum;

void TestSolarMath::testExcelDateValue() {
    qint64 n = excelDateValue(QDate(2024,1,11));
    // Excel formula: DATEVALUE("11/1/2024")
    QCOMPARE(n, 45302);
}

void TestSolarMath::testCalcSun() {
    Sun sun = calcSun(55.32, 11.40, QDateTime(QDate(2024,9,9), QTime(9,57,00), QTimeZone("UTC+01:00")), 1.);
    // From https://gml.noaa.gov/grad/solcalc/index.html
    QTime
        sunrise(5,36,0),
        noon(12,11,46),
        sunset(18,46,00);
    double
        azimuth = 138.85,
        elevation = 33.01;
    QVERIFY(abs(sun.sunrise.secsTo(sunrise)) < 30);
    QVERIFY(abs(sun.noon.secsTo(noon)) < 30);
    QVERIFY(abs(sun.sunset.secsTo(sunset)) < 60);
    QVERIFY(eq(sun.azimuth, azimuth, 1e-2));
    QVERIFY(eq(sun.elevation, elevation, 1e-2));
}


