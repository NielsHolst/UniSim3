/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
ï»¿#include <base/publish.h>
#include "lin_dev_rate.h"
#include <math.h>

using namespace base;

namespace bmsb {

PUBLISH(LinDevRate)

LinDevRate::LinDevRate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("estimate the stage-specific daily developmental rate using linear model; later replace daily rate to hourly rate");
        Input(dailytempC).equals(17).help("Temperature (Â°C)");
        Input(a0).equals(-0.03248936).help("Estimate of constant a0");
        Input(a1).equals(0.00228921).help("Estimate of slope coefficient a1)");
        Output(lineardevrate).help ("devrate refers to mean development rate (1/day)");
}

void LinDevRate::reset() {
        update();
}

void LinDevRate::update() {
    help("hourly temperature will later replace daily temp to estimate linear model based dev rate");
    help("Note that input parameter must be supplied in degree Celsius for Temperature");
//      Linear model : (rT ~ a0 + a1 * T )
        lineardevrate     = ((a0) +  (a1 * dailytempC));
}


}

