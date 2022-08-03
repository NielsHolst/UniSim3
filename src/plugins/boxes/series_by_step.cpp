/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "series_by_step.h"

using namespace base;

namespace boxes {

PUBLISH(SeriesByStep)

SeriesByStep::SeriesByStep(QString name, Box *parent)
    : SeriesBase(name, parent)
{
    help("produces a series of numbers changing by every step");
    Output(steps).noClear().help("The number of steps needed to run through all values");
}

void SeriesByStep::setCounter() {
    setCounterByStep();
}

void SeriesByStep::setSteps() {
    // We need 1 reset + (numValues - 1) updates to go through all values
    steps = numValues - 1;
}

void SeriesByStep::reset () {
    resetByStep();
}

void SeriesByStep::update () {
    updateByStep();
}

}
