/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
ï»¿#include <base/publish.h>
#include "extremeval_func.h"
#include <math.h>

using namespace base;

namespace bmsb {

PUBLISH(ExtremeValFunc)

ExtremeValFunc::ExtremeValFunc(QString name, QObject *parent)
    : Box(name, parent)
{
    help("estimate the Extreme Vaue Function;later replace daily rate to hourly rate");
        Input(dailytempC).equals(17).help("Temperature (Â°C)");
        Input(k1).equals(228.61208).help("Estimate of parameter k1, maximum value for dependent parameter)");
        Input(Tmax).equals(-15.04058).help("Estimate of parameter Tmax, opt temp at which dependent parameter is maximized");
        Input(rho).equals(0.25721).help("Estimate of fitted constant, rho");
        Output(evfout).help ("Output from the evf model");
}
void ExtremeValFunc::reset() {
        update();
}

void ExtremeValFunc::update() {
    help("hourly temperature will later replace daily temp to estimate parameter of interest using Extreme Vaue Function");
    help("Note that input parameter must be supplied in degree Celsius for Temperature");
//      ExtremeValueFunc model : k* exp (1 + (Tmax-dailytempC)/rho-exp((Tmax-dailytempC)/rho));
//      Source: https://doi.org/10.1111/j.1461-9555.2005.00260.x;
        evfout     =  (k1 * exp(1 + (Tmax-dailytempC)/rho-exp((Tmax-dailytempC)/rho)));
}


}

