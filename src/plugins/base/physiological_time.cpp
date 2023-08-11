/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "physiological_time.h"

namespace base {

PhysiologicalTime::PhysiologicalTime(QString name, Box *parent)
    : Box(name, parent) {
    Input(T).unit("oC").imports("weather[Tavg]");
    Input(timeStepDays).unit("d").imports("calendar[timeStepDays]");
    Input(resetTotal).equals(false).help("Reset `total` zero?");
    Input(isTicking).equals(true).help("Is the physiological time running? Else `step` will be zero");
    Output(step).unit("PTU").help("Increment in physiological time units (PTU)");
    Output(total).unit("PTU").help("Total time since reset");
}

void PhysiologicalTime::update() {
    if (!isTicking)
        step = 0.;
    else
        updateStep();
    if (resetTotal)
        total = 0;
    total += step;
}

} //namespace


