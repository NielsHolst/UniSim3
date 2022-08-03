/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
ï»¿#include <base/publish.h>
#include "exppoly2_mort_rate.h"
#include <math.h>

using namespace base;

namespace bmsb {

PUBLISH(ExpPoly2MortRate)

ExpPoly2MortRate::ExpPoly2MortRate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("estimate the stage-specific daily survival rate using ExpPoly2 (ExpPolynomial) model and convert to mortality rate; later replace daily rate to hourly rate");
        Input(dailytempC).equals(17).help("Temperature (Â°C)");
        Input(a0).equals(13.41368).help("Estimate of coefficient a0)");
        Input(a1).equals(-1.25998).help("Estimate of coefficient a1");
        Input(a2).equals(0.02423).help("Estimate of coefficient a2");
        Output(survrate).help ("survrate refers to mean survival rate (1/day)");
        Output(mortrate).help ("mortrate refers to mean mortality rate (1/day)");
}

void ExpPoly2MortRate::reset() {
        update();
}

void ExpPoly2MortRate::update() {
    help("hourly temperature will later replace daily temp to estimate exppoly2 model based surv rate");
    help("Note that input parameter must be supplied in degree Celsius for Temperature");
//      ExpPoly2 model as in devRate packege in R: (rT ~ a0 + a1 * T + a2 * T^2)
        survrate     = 1-exp((a0) +  (a1 * dailytempC) + (a2 * pow(dailytempC,2)));
        mortrate     = 1-survrate;
}


}

