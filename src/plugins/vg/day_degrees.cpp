/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "day_degrees.h"

using namespace base;

namespace vg {

PUBLISH(DayDegrees)

DayDegrees::DayDegrees(QString name, Box *parent)
    : PhysiologicalTime(name, parent)
{
    Input(T0).equals(0);
    Input(Topt).equals(100);
    Input(Tmax).equals(100);
    Input(T).imports("weather[Tavg]",CA);
}

double DayDegrees::calcDailyTimeStep()
{
    double step;
    if (T < T0)
        step = 0.;
    else if (T < Topt)
        step = T - T0;
    else if (T < Tmax)
        step = Tmax - T;
    else
        step = 0;
    return step;
}

} //namespace

