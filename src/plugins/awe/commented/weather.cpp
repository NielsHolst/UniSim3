/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <boost/math/constants/constants.hpp>
#include <base/publish.h>
#include "weather.h"

using namespace base;

namespace awe {

PUBLISH(Weather)

Weather::Weather(QString name, QObject *parent)
    : Box(name,parent)
{ 
    Input(dayOfYear).imports("calendar[dayOfYear]");
    Input(Tmin).equals(-0.75).help("Yearly temperature minimum (oC)");
    Input(max).equals(17.20).help("Yearly temperature maximum (oC)");
    Input(offset).equals(30).help("Offset of minimum from 1 January (days)");
    Output(T).help("Daily average temperature oC)");
}

void Weather::reset()
{
    T = temperature(0);
}

void Weather::update()
{
    T = temperature(dayOfYear);
}

double Weather::temperature(int dayOfYear) const {
    const double PI = boost::math::constants::pi<double>();
    double k = sin(PI*(dayOfYear-offset)/365.);
    return Tmin + max*k*k;
}

}
