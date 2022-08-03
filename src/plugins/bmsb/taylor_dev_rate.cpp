/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
ï»¿#include <base/publish.h>
#include "taylor_dev_rate.h"
using namespace base;

namespace bmsb {

PUBLISH(TaylorDevRate)

TaylorDevRate::TaylorDevRate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("estimate the stage-specific daily developmental rate using Taylor model; later replace daily rate to hourly rate");
        Input(dailytempC).equals(17).help("Temperature (Â°C)");
        Input(Rm).equals(0.132).help("Estimate of coefficient Rm)");
        Input(Tm).equals(30.555).help("Estimate of coefficient Tm");
        Input(T0).equals(8.941).help("Estimate of coefficient T0");
        Output(devrate).help ("devrate refers to mean development rate (1/day)");
}

void TaylorDevRate::reset() {
    update();
}

void TaylorDevRate::update() {
    help("hourly temperature will later replace daily temp to estimate Taylor model based dev rate");
    help("Note that input parameter must be supplied in degree Celsius for Temperature");
//      Taylor model as in devRate packege in R: (rT ~ Rm * exp(-1/2 * ((T - Tm)/To)^2))
        devrate     = ((Rm) *  exp((-1/2) * (((dailytempC - Tm)/T0) *((dailytempC - Tm)/T0))));
}


}

