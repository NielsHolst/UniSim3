/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "weather.h"

using namespace base;

namespace boxes {

PUBLISH(Weather)

Weather::Weather(QString name, Box *parent)
    : Box(name, parent)
{
    Input(Tmin).importsMaybe("descendants::*[Tmin]");
    Input(Tmax).importsMaybe("descendants::*[Tmax]");
    Output(Tavg);
}

void Weather::reset() {
    update();
}

void Weather::update() {
    Tavg = (Tmin + Tmax)/2.;
}

}
