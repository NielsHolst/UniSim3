/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Andrew Antaya, University of Arizona [aantaya at email.arizona.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "Birth.h"

using namespace base;

namespace pinacate {

PUBLISH(Birth)

Birth::Birth(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(birthday).help("the Julian date on which cohort is born");
    Input(fecundity).help("reproductive output of adults");
    Input(currentday).help("the current day in the simulation");

    Output(value);
}

void Birth::reset() {
}

void Birth::update() {
// the birth-pulse of new juveniles into the model
    if (currentday == birthday) value = fecundity;
    else value = 0;
}


}
