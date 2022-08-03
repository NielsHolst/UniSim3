/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "series_by_iteration.h"

using namespace base;

namespace boxes {

PUBLISH(SeriesByIteration)

SeriesByIteration::SeriesByIteration(QString name, Box *parent)
    : SeriesBase(name, parent)
{
    help("produces a given series of numbers changing by every iteration");
    Output(iterations).noClear().help("The number of iterations needed to run through all values");
}

void SeriesByIteration::setCounter() {
    setCounterByIteration();
}

void SeriesByIteration::setIterations() {
    iterations = numValues;
}

void SeriesByIteration::reset () {
    resetByIteration();
}

}
