/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "proportional_signal.h"

using namespace base;

namespace boxes {
	
PUBLISH(ProportionalSignal)

ProportionalSignal::ProportionalSignal(QString name, Box *parent)
    : SlidingSignal(name, parent)
{
    help("produces a signal value that responds proportionally to the input value");
}

double ProportionalSignal::slide(double proportion) {
    return proportion;
}

} //namespace


