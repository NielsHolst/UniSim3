/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SOLAR_MATH_H
#define SOLAR_MATH_H
#include <QDate>
#include <QTime>

namespace phys_math {

struct Sun {
    QTime
        sunrise, noon, sunset, solarTime;
    double
        dayLength, // hours
        azimuth,   // degrees [0;360]; north = 0
        elevation; // angle above [0;90] or below [-90,0] the horizon

};

Sun calcSun(double latitude, double longitude, QDateTime dateTime, double timeZone);

qint64 excelDateValue(QDate date);
// Excel codes a date as days since 1/1/1900, pretending 1900 was a leap year

double extraterrestrialFlux(QDate date);
// W/m2 at the top of the atmosphere

double clearSkyTransmittance(double sunElevation, double altitude);
// Optimal W/m2 at the soil surface

double angleOfIncidence(double sunElevation, double sunAzimuth, double surfaceSlope, double surfaceAzimuth);



} //namespace


#endif
