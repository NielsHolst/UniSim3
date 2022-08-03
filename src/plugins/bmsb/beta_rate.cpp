/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
ï»¿#include <base/publish.h>
#include "beta_rate.h"
#include <math.h>

using namespace base;

namespace bmsb {

PUBLISH(BetaRate)

BetaRate::BetaRate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("estimate the stage-specific daily developmental rate using Briere2 model; later replace daily rate to hourly rate");
        Input(dailytempC).equals(17).help("Temperature (Â°C)");
        Input(mu).equals(-3.6875).help("mu");
        Input(Tb).equals(19.5904).help("Estimate of basal temperature)");
        Input(Tc).equals(36.777).help("Estimate of ceiling temperature");
        Input(aa).equals(0.3961).help("Estimate of aa");
        Input(bb).equals(0.1967).help("Estimate of bb");
        Output(ovprate).help ("apoprate refers to adult pre-oviposition or oviposition rate (1/day)");
        }

void BetaRate::reset() {
        update();
}

void BetaRate::update() {
        help("hourly temperature will later replace daily temp to estimate Beta model based rate");
        help("Note that input parameter Tb and Tc must be supplied in degree Celsius for Temperature");
//      Beta model as in devRate packege in R: (rT = exp(mu) * (T - Tb)^aa * (Tc - T)^bb)
        ovprate     = (exp(mu) *  pow((dailytempC - Tb), (aa)) * pow((Tc - dailytempC),(bb)));
}


}

