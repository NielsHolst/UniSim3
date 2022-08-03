/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
/* Day length and solar elevation equations copied from the FORTRAN code of Kroppf &
** Laar (1993). Modelling Crop-Weed Interactions. CAB International.
*/
#include <cfloat>
#include <cmath>
#include <QTextStream>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "solar_time.h"

using namespace std;
using namespace base;
using namespace phys_math;

namespace boxes {

constexpr double RAD = PI/180.;
constexpr double DEGREES = 1./RAD;

PUBLISH(SolarTime)

SolarTime::SolarTime(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes true solar time and azimuth");
    Input(latitude).imports("calendar[latitude]");
    Input(longitude).imports("calendar[longitude]");
    Input(dayOfYear).imports("calendar[dayOfYear]");
    Input(time).imports("calendar[time]");
    Input(timeZone).imports("calendar[timeZone]");
    Output(trueSolarTime).help("Current true solar time of the day").unit("h:m:s");
    Output(azimuth).help("The compass direction of the sun relative to north").unit("[-180;180]");
}

void SolarTime::reset() {
    update();
}

void SolarTime::update() {
    // First, the fractional year y is calculated, in radians.
    double y = 2*PI*(dayOfYear-1+(time.hour()-12.)/24)/365.;

    //From y, we can estimate the equation of time (in minutes) and the solar declination angle (in radians).
    double eqtime =
    229.18*( 0.000075+0.001868*cos(y)-0.032077*sin(y)-0.014615*cos(2*y)-0.040849*sin(2*y) );

    double declin = 0.006918-0.399912*cos(y)+0.070257*sin(y)
            -0.006758*cos(2*y)+0.000907*sin(2*y)
            -0.002697*cos(3*y)+0.00148*sin(3*y);

    // Next, the true solar time is calculated in the following two equations. First the time offset is
    // found, in minutes, and then the true solar time, in minutes.
    double timeOffset = eqtime - 4*longitude + 60*timeZone; // minutes
    // where eqtime is in minutes, longitude is in degrees, timezone is in hours from UTC

    // True solar time
    double tst = time.hour()*60 + time.minute() + time.second()/60. + timeOffset; // minutes
    trueSolarTime = QTime(0,0,0).addSecs(int(tst+0.5)*60);
//    trueSolarTime = QTime::fromMSecsSinceStartOfDay(int(tst+0.5)*60*1000);
    if (!trueSolarTime.isValid())
        trueSolarTime = QTime(0,0);

    // The solar hour angle, in radians, is:
    double ha = (tst/4 - 180)*RAD;

    // The solar zenith angle (phi) can then be found from the following equation:
    double lat = latitude*RAD;
    double cosPhi = sin(lat)*sin(declin) + cos(lat)*cos(declin)*cos(ha);
    double phi = acos(cosPhi);

    // And the solar azimuth is:
    double az1 = -(sin(lat)*cos(phi)-sin(declin))/(cos(lat)*sin(phi));
    double az2 = acos(az1)*DEGREES;
    azimuth = az2-90.;
}


} //namespace

