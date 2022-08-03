/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
ï»¿#include <base/publish.h>
#include "poly3_rate.h"
#include <math.h>

using namespace base;

namespace bmsb {

PUBLISH(Poly3Rate)

Poly3Rate::Poly3Rate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("estimate the stage-specific daily developmental rate using Poly3 (Polynomial) model; later replace daily rate to hourly rate");
        Input(dailytempC).equals(17).help("Temperature (Â°C)");
        Input(a0).equals(3.472e+00).help("Estimate of coefficient a0)");
        Input(a1).equals(-5.624e-01).help("Estimate of coefficient a1");
        Input(a2).equals(3.211e-02).help("Estimate of coefficient a2");
        Input(a3).equals(-7.403e-04).help("Estimate of coefficient a3");
        Output(survrate).help ("survrate refers to mean survival rate (1/day)");
}

void Poly3Rate::reset() {
        update();
}

void Poly3Rate::update() {
    help("hourly temperature will later replace daily temp to estimate poly3 model based rate");
    help("Note that input parameter must be supplied in degree Celsius for Temperature");
//  Poly3 model as in devRate packege in R: (rT ~ a0 + a1 * T + a2 * T^2 + a3 * T^3 + a4 * T^4)
//  We modeled survival. Hence survival rate is:
    survrate     = ((a0) +  (a1 * dailytempC) + (a2 * pow(dailytempC,2)) + (a3 * pow(dailytempC,3)));
}


}

