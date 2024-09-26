/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/

#include <cmath>
#include <base/phys_math.h>
#include <base/solar_math.h>
#include <base/publish.h>
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
    Input(longitude).imports("calendar[longitude]");
    Input(dateTime).imports("calendar[dateTime]");
    Input(timeZone).imports("calendar[timeZone]");
    Input(altitude).help("Altitude above sea level").unit("m");

    Output(sunrise).help("Time of sunrise").unit("time");
    Output(noon).help("Time of nonw").unit("time");
    Output(sunset).help("Time of sunset").unit("time");
    Output(solarTime).help("Solar time (noon=12:00:00").unit("time");

    Output(dayLength).help("Day length from sunrise to sunset").unit("h");
    Output(elevation).help("Solar angle to horizon").unit("[-90;90]");
    Output(azimuth).help("Compass direction to sun (north=0)").unit("[0;360]");
    Output(isDay).help("Is the sun up?").unit("bool");
    Output(isNight).help("Has the sun set?").unit("bool");

    Output(solarConstant).help("The irradiation at the top of the atmosphere").unit("W/m2");
    Output(angot).help("The irradiation on Earth surface under optimal atmospheric conditions").unit("W/m2");
}

void Sun::reset() {
    update();
}

void Sun::update() {
    auto sun = calcSun(latitude, longitude, dateTime, timeZone);
    sunrise = sun.sunrise;
    noon = sun.noon;
    sunset = sun.sunset;
    solarTime = sun.solarTime;
    dayLength = sun.dayLength;
    elevation = sun.elevation;
    azimuth = sun.azimuth;
    isDay = (elevation > 0.);
    isNight = !isDay;
    solarConstant = extraterrestrialFlux(dateTime.date());
    angot = solarConstant*clearSkyTransmittance(elevation, altitude);
}

} //namespace

