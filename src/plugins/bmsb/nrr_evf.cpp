/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
ï»¿#include <base/publish.h>
#include "nrr_evf.h"
#include <math.h>

using namespace base;

namespace bmsb {

PUBLISH(NrrEVF)

NrrEVF::NrrEVF(QString name, QObject *parent)
    : Box(name, parent)
{
    help("estimate the Net Reproductive Rate using NrrEVF (Extreme Vaue Function) model; later replace daily rate to hourly rate");
        Input(dailytempC).equals(17).help("Temperature (Â°C)");
        Input(a).equals(228.61208).help("Estimate of coefficient a0)");
        Input(b).equals(-15.04058).help("Estimate of coefficient a1");
        Input(k).equals(0.25721).help("Estimate of coefficient a2");
        Output(nrrout).help ("Net Reproductive Rate");
}
void NrrEVF::reset() {
        update();
}

void NrrEVF::update() {
    help("hourly temperature will later replace daily temp to estimate NRR based on NrrEVF (Extreme Vaue Fnction) model ");
    help("Note that input parameter must be supplied in degree Celsius for Temperature");
//      NrrEVF model : a* exp (1 + (b-dailytempC)/k-exp ((b-dailytempC)/k))
        nrrout     = a * exp(1 + (b-dailytempC)/k-exp((b-dailytempC)/k));
}


}

