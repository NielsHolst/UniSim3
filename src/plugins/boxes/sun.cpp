/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
/* Day length and solar elevation equations copied from the FORTRAN code of Kroppf &
** Laar (1993). Modelling Crop-Weed Interactions. CAB International.
*/
#include <cmath>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "sun.h"

using namespace std;
using namespace base;
using namespace phys_math;

namespace boxes {

constexpr double RAD = PI/180.;

PUBLISH(Sun)

Sun::Sun(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes sun-earth astronomy");
    Input(latitude).imports("calendar[latitude]");
    Input(dayOfYear).imports("calendar[dayOfYear]");
    Input(time).imports("calendar[time]");
    Output(dayLength).help("Astronomic day length").unit("h");
    Output(sinb).help("Sine of solar height over the horizon").unit("[-1;1]");
    Output(sunrise).help("Time of sunrise").unit("h:m:s");
    Output(sunset).help("Time of sunset").unit("h:m:s");
    Output(solarConstant).help("The irradiation at the top of the atmosphere").unit("W/m2");
    Output(angot).help("The irradiation on Earth surface under optimal atmospheric conditions").unit("W/m2");
    Output(isDay).help("Tells of it's between sunrise and sunset").unit("bool");
    Output(isNight).help("Tells of it's between sunset and sunrise").unit("bool");
}

void Sun::reset() {
    update();
}

void Sun::update() {
/* See
 *  http://www.marsop.info/marsopdoc/metamp/05010401.HTM
 */
    double dec = -asin(sin(23.45*RAD)*cos(2*PI*(dayOfYear+10.)/365.)),
           sinLD = sin(RAD*latitude)*sin(dec),
           cosLD = cos(RAD*latitude)*cos(dec);
    Q_ASSERT(TestNum::neZero(cosLD));
    double aob = sinLD/cosLD;
    if (aob > 1) aob = 1.;
    if (aob < -1) aob = -1.;
    dayLength = 12.*(1. + 2.*asin(aob)/PI);
    int halfDay = dayLength/2.*60.*60.;
    sunrise = QTime(12,00).addSecs(-halfDay); // Shouldn't solar noon be used?
    sunset = QTime(12,00).addSecs(halfDay);

    double h = time.hour() + time.minute()/60. + time.second()/3600.;
    sinb = sinLD + cosLD*cos(2.*PI*(h + 12.)/24.);
    if (sinb < 0.) sinb = 0.;
    // From Kropff & Laar (1993), pp. 235-236
    double dsinb = 3600.*(dayLength*sinLD + 24.*cosLD*sqrt(1. - aob*aob)/PI);
    double dsinbe = 3600.*(dayLength*(sinLD + 0.4*(sinLD*sinLD + cosLD*cosLD*0.5)) +
                           12.*cosLD*(2. + 3.*0.4*sinLD)*sqrt(1. - aob*aob)/PI);
    solarConstant = 1370.*(1. + 0.033*cos(2.*PI*dayOfYear/365.));
    angot = solarConstant*dsinb*sinb*(1. + 0.4*sinb)/dsinbe; // This is symmetrical around noon. So, shouldn't solar time be used?

    isNight = (time > sunset) || (time < sunrise);
    isDay = !isNight;
}

} //namespace

