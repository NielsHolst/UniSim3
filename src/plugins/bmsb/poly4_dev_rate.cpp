/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
ï»¿#include <base/publish.h>
#include "poly4_dev_rate.h"
#include <math.h>

using namespace base;

namespace bmsb {

PUBLISH(Poly4DevRate)

Poly4DevRate::Poly4DevRate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("estimate the stage-specific daily developmental rate using Poly4 (Polynomial) model; later replace daily rate to hourly rate");
        Input(dailytempC).equals(17).help("Temperature (Â°C)");
        Input(a0).equals(3.472e+00).help("Estimate of coefficient a0)");
        Input(a1).equals(-5.624e-01).help("Estimate of coefficient a1");
        Input(a2).equals(3.211e-02).help("Estimate of coefficient a2");
        Input(a3).equals(-7.403e-04).help("Estimate of coefficient a3");
        Input(a4).equals(5.927e-06).help("Estimate of coefficient a4");
        Output(devrate).help ("devrate refers to mean development rate (1/day)");
}

void Poly4DevRate::reset() {
        update();
}

void Poly4DevRate::update() {
    help("hourly temperature will later replace daily temp to estimate poly4 model based dev rate");
    help("Note that input parameter must be supplied in degree Celsius for Temperature");
//      Poly4 model as in devRate packege in R: (rT ~ a0 + a1 * T + a2 * T^2 + a3 * T^3 + a4 * T^4)
        devrate     = ((a0) +  (a1 * dailytempC) + (a2 * pow(dailytempC,2)) + (a3 * pow(dailytempC,3)) + (a4 * pow(dailytempC,4)));
}


}

