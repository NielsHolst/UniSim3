/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "day_degrees.h"
using namespace base;

namespace boxes {

PUBLISH(DayDegrees)

DayDegrees::DayDegrees(QString name, Box *parent)
    : PhysiologicalTime(name, parent) {
    help("computes linear day-degrees");
    Input(T0).equals(0).unit("oC").help("Lower temperature threshold");
    Input(Topt).equals(100).unit("oC").help("Optimum temperate; linear decline from here to `Tmax`");
    Input(Tmax).equals(100).unit("oC").help("Upper temperature threshold");
    port("step")->unit("oD").help("Increment in day-degrees");
    port("total")->unit("oD");
}

void DayDegrees::reset() {
    if (T0 >= Topt) {
        QString msg{"T0 (%1) must be smaller than Topt (%2)"};
        ThrowException(msg.arg(T0).arg(Topt)).context(this);
    }
    if (Topt > Tmax) {
        QString msg{"Topt (%1) cannot be larger than Tmax (%2)"};
        ThrowException(msg.arg(Topt).arg(Tmax)).context(this);
    }
}

void DayDegrees::updateStep() {
    if (T <= T0)
        step = 0.;
    else if (T <= Topt)
        step = T - T0;
    else if (T >= Tmax)
        step = 0;
    else
        step = (Topt - T0)*(Tmax - T)/(Tmax - Topt);
    step *= timeStepDays;
}

} //namespace

