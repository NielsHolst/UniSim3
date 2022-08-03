/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "sequence_by_iteration.h"

using namespace base;

namespace boxes {

PUBLISH(SequenceByIteration)

SequenceByIteration::SequenceByIteration(QString name, Box *parent)
    : SequenceBase(name, parent)
{
    help("produces a sequence of numbers changing by every iteration");
    Output(iterations).noClear().help("The number of iterations needed to run through all values");
}

void SequenceByIteration::setCounter() {
    setCounterByIteration();
}

void SequenceByIteration::setIterations() {
    iterations = numValues;
}

void SequenceByIteration::reset () {
    resetByIteration();
}

}
