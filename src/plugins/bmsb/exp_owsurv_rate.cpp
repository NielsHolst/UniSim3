/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
ï»¿#include <base/publish.h>
#include "exp_owsurv_rate.h"
#include <math.h>

using namespace base;

namespace bmsb {

PUBLISH(ExpOWSurvRate)

ExpOWSurvRate::ExpOWSurvRate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("estimate the daily survival rate using Exp model; later replace daily rate to hourly rate");
        Input(dailytempC).equals(-17).help("Temperature (Â°C)");
        Input(a).equals(1.9902546174469).help("Estimate of the coefficient a)");
        Input(b).equals(0.0521511391227991).help("Estimate of the coefficient b");
        Input(c).equals(1.33756227531161).help("Estimate of the coefficient c");
        Input(t).equals(0.041677).help("Estimate of the parameter for time in days, t");
        Output(owsurvrate).help ("owsurvrate refers to over winter survival rate");
}

void ExpOWSurvRate::reset() {
        update();
}

void ExpOWSurvRate::update() {
    help("hourly temperature will later replace daily temp to estimate exp model based owsurv rate");
    help("Note that input parameter must be supplied in degree Celsius for Temperature");
//      Exp model as in Nedved 1998 article: (S(t, T) = exp(a+bt(T-c))/(1 + exp(a+bt(T-c))))
        dailytempC = dailytempC + 5;
        owsurvrate     = exp(a+b*t*(dailytempC-c))/(1+exp(a+b*t*(dailytempC-c)));
}


}

