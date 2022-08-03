/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Merijn Moens, Aarhus University [merijnmoens at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "fecundity.h"

using namespace base;

namespace apis {

PUBLISH(Fecundity)

Fecundity::Fecundity(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes queen fecundity");
    Input(maxFecundity).help("Max. queen fecundity (eggs per day)");
    Input(slope).help("Slope of brood number on log worker number (after Harbo 1986");
    Input(intercept).help("Intercept of brood number on log worker number (after Harbo 1986");
    Input(brood).imports("egg[number]|larva[number]|capped[number]").transform(Sum).help("Number of brood");
    Input(workers).imports("houserkeeper[number]|forager[number]").transform(Sum).help("Number of workers");
    Input(timeStepDays).imports("calendar[timeStepDays]");
    Output(value).help("Number of eggs laid in this time step (#eggs)");
    Output(rate).help("Queen fecundity (eggs per day)");
}

void Fecundity::update() {
    if (workers == 0.) {
        rate = 0.;
    }
    else {
        double broodDemand = slope*log(workers) + intercept,
               fecundityDemand = (broodDemand - brood)/timeStepDays;
        if (fecundityDemand <= 0.)
            rate = 0.;
        else if (fecundityDemand < maxFecundity)
            rate = fecundityDemand;
        else
            rate = maxFecundity;
    }
    value = rate*timeStepDays;
}

}
