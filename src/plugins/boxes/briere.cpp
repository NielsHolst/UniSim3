/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/exception.h>
#include <base/publish.h>
#include "briere.h"
using namespace base;

namespace boxes {

PUBLISH(Briere)

Briere::Briere(QString name, Box *parent)
    : PhysiologicalTime(name, parent) {
    help("computes non-linear development rate per day");
    Input(T0).unit("oC").help("Lower temperature threshold");
    Input(Tmax).unit("oC").help("Upper temperature threshold");
    Input(a).unit("/oC").help("Scaling parameter");
}

void Briere::reset() {
    if (T0 >= Tmax) {
        QString msg{"T0 (%1) must be smaller than Tmax (%2)"};
        ThrowException(msg.arg(T0).arg(Tmax)).context(this);
    }
}

void Briere::updateStep() {
    if (!isTicking)
        step = 0.;
    else if (T <= T0)
        step = 0.;
    else if (T < Tmax)
        step = a*T*(T-T0)*sqrt(Tmax-T);
    else
        step = 0;
    step *= timeStepDays;
}

} //namespace

