/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
ï»¿#include <base/publish.h>
#include "fecundity_evf.h"
#include <math.h>

using namespace base;

namespace bmsb {

PUBLISH(FecundityEVF)

FecundityEVF::FecundityEVF(QString name, QObject *parent)
    : Box(name, parent)
{
    help("estimate the fecundity using FecundityEVF (Extreme Vaue Fnction) model; later replace daily rate to hourly rate");
        Input(dailytempC).equals(17).help("Temperature (Â°C)");
        Input(a).equals(228.61208).help("Estimate of coefficient a0)");
        Input(b).equals(-15.04058).help("Estimate of coefficient a1");
        Input(k).equals(0.25721).help("Estimate of coefficient a2");
        Output(fecundity).help ("fecundity refers to number of eggs laid/female in life cycle (days)");
}
void FecundityEVF::reset() {
        update();
}

void FecundityEVF::update() {
    help("hourly temperature will later replace daily temp to estimate fecundity based on FecundityEVF (Extreme Vaue Fnction) model ");
    help("Note that input parameter must be supplied in degree Celsius for Temperature");
//      FecundityEVF model : a* exp (1 + (b-dailytempC)/k-exp ((b-dailytempC)/k))
        fecundity     = a * exp(1 + (b-dailytempC)/k-exp((b-dailytempC)/k));
        fecundity     = fecundity/2.0;
}


}

