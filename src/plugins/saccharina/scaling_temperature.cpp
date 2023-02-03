/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "scaling_temperature.h"

using namespace base;

namespace saccharina {

PUBLISH(ScalingTemperature)

ScalingTemperature::ScalingTemperature(QString name, Box *parent)
    : Box(name, parent)
{
    help("scales growth demand and photosynthesis by temperature");
    Input(T).unit("oC").help("Current temperature");
    Input(Tmin). equals(-2).unit("oC").help("Minimum temperature");
    Input(Topt1).equals( 5).unit("oC").help("Lower temperature optimum");
    Input(Topt2).equals(15).unit("oC").help("Higher temperature optimum");
    Input(Tmax). equals(24).unit("oC").help("Maximum temperature");
    Output(value).unit("[0;1]").help("Scaling factor");
}

void ScalingTemperature::reset() {
   update();
}

void ScalingTemperature::update() {
    // Note: Inequalities rule out division by zero
    if (T < Tmin)
        value = 0.;
    else if (T < Topt1)
        value = (T-Tmin)/(Topt1-Tmin);
    else if (T < Topt2)
        value = 1.;
    else if (T < Tmax)
        value = (Tmax-T)/(Tmax-Topt2);
    else
        value = 0.;
}

}
