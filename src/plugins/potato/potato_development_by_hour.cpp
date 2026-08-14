/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Isaac Kwesi Abuley , Aarhus University [ikabuley at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "potato_development_by_hour.h"
//modelling the physiologic age of potatoes on hourly basis
//the formula is taken from the Altsim 1.0 model by Jean Robert Pelletier (1987)

using namespace base;

namespace potato {

PUBLISH(PotatoDevelopmentByHour)

PotatoDevelopmentByHour::PotatoDevelopmentByHour(QString name, QObject *parent)
    : PotatoDevelopmentBase(name, parent)
{
    help("models development based on hourly temperature (Altsim 1.0, Pelletier, 1987)");
    Input(temp).imports("weather[Temp]").help("The hourly temperature");
}

void PotatoDevelopmentByHour::reset() {
    total = 0;
    update();
}

inline double sqr(double x) { return x*x; }

void PotatoDevelopmentByHour::update()
{
    if (temp < 7.)
        step = 0;
    else if (temp < 21.)
        step = 0.416667*(1-sqr(temp-21.)/196);
    else if (temp < 30.)
        step = 0.416667*(1-sqr(temp-21.)/81);
    else
        step = 0.;
    step *= 24.*timeStepDays;
    total += step;
}

}
