/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
ï»¿#include <base/publish.h>
#include "briere2_dev_rate.h"
#include <math.h>

using namespace base;

namespace bmsb {

PUBLISH(Briere2DevRate)

Briere2DevRate::Briere2DevRate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("estimate the stage-specific daily developmental rate using Briere2 model; later replace daily rate to hourly rate");
        Input(dailytempC).equals(17).help("Temperature (Â°C)");
        Input(aa).equals(2.056e-04).help("Estimate of coefficient aa)");
        Input(bb).equals(1.031e+01).help("Estimate of coefficient bb");
        Input(Tmin).equals(8.888e+00).help("Estimate of coefficient Tmin");
        Input(Tmax).equals(3.605e+01).help("Estimate of coefficient Tmax");
        Output(devrate).help ("devrate refers to mean development rate (1/day)");
}

void Briere2DevRate::reset() {
        update();
}

void Briere2DevRate::update() {
		help("hourly temperature will later replace daily temp to estimate Briere2 model based dev rate");
		help("Note that input parameter must be supplied in degree Celsius for Temperature");
//      Briere2 model as in devRate packege in R: (rT ~ aa * T * (T - Tmin) * (Tmax - T)^(1/bb))
        devrate     = ((aa) *  (dailytempC) * (dailytempC - Tmin) * pow((Tmax - dailytempC),(1/bb)));
}


}

