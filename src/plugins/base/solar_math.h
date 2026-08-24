/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SOLAR_MATH_H
#define SOLAR_MATH_H

#include <QDate>
#include <QTime>

#include "base_dllexport.h"

namespace phys_math {

struct UNI_API Sun {
    QTime
        sunrise, noon, sunset, solarTime;
    double
        dayLength, // hours
        azimuth,   // degrees [0;360]; north = 0
        elevation; // angle above [0;90] or below [-90,0] the horizon

};

UNI_API Sun calcSun(double latitude, double longitude, QDateTime dateTime, double timeZone);

UNI_API qint64 excelDateValue(QDate date);
// Excel codes a date as days since 1/1/1900, pretending 1900 was a leap year

UNI_API double extraterrestrialFlux(QDate date);
// W/m2 at the top of the atmosphere

UNI_API double clearSkyTransmittance(double sunElevation, double altitude);
// Optimal W/m2 at the soil surface

UNI_API double angleOfIncidence(double sunElevation, double sunAzimuth, double surfaceSlope, double surfaceAzimuth);

} //namespace


#endif
