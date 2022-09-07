/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <limits>
#include <base/publish.h>
#include "maximum_at.h"

using namespace base;

namespace boxes {

PUBLISH(MaximumAt)

MaximumAt::MaximumAt(QString name, Box *parent)
    : Box(name, parent)
{
    help("keeps maximum value and its step");
    Input(step).imports("/.[step]");
    Input(track).help("Value to track for max value");
    Output(at).help("Step in which maximum value was attained");
    Output(value).help("The maximum value attained");
}

void MaximumAt::reset() {
    at = step;
    value = track;
}

void MaximumAt::update() {
    if (track > value) {
        at = step;
        value = track;
    }
}

} //namespace

