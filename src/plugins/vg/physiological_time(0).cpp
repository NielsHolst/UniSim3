/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/time_with_units.h>
#include "physiological_time.h"

using namespace base;

namespace vg {

PhysiologicalTime::PhysiologicalTime(QString name, Box *parent)
    : Box(name, parent)
{
    setClassName("PhysiologicalTime");
    Input(step);
    Input(total);
    Input(timeStepDays).imports("calendar[timeStepDays]");
    Input(doReset).equals(false);
    Input(isTicking).equals(true);
}

void PhysiologicalTime::update() {
    if (doReset) {
        step = total = 0.;
    }
    if (isTicking) {
        step = calcDailyTimeStep()*timeStepDays;
        total += step;
    }
}

} //namespace

