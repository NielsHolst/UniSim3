/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "sequence_by_step.h"

using namespace base;

namespace boxes {

PUBLISH(SequenceByStep)

SequenceByStep::SequenceByStep(QString name, Box *parent)
    : SequenceBase(name, parent)
{
    help("produces a sequence of numbers changing by every step");
    Output(steps).noClear().help("The number of steps needed to run through all values");
}


void SequenceByStep::setCounter() {
    setCounterByStep();
}

void SequenceByStep::setSteps() {
    // We need 1 reset + (numValues - 1) updates to go through all values
    steps = numValues - 1;
}

void SequenceByStep::reset () {
    resetByStep();
}

void SequenceByStep::update () {
    updateByStep();
}

}
