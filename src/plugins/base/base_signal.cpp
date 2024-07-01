/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include "base_signal.h"
#include "phys_math.h"
#include "test_num.h"

using phys_math::infinity;

namespace base {

BaseSignal::BaseSignal(QString name, Box *parent)
    : Box(name, parent)
{
    setClassName("base", "BaseSignal");
    help("is a base class classes that compute signal");
    Input(initialSignal).help("Value of `signal` at reset");
    Input(initialFlag).equals(false).help("Value of flag at reset");
    Output(signal).help("Value of the signal");
    Output(flagIsUp).help("Is the `signal` on?");
    Output(flagJustRaised). help("Was the `signal` just set on?");
    Output(flagJustLowered).help("Was the `signal` just set off?");
    NamedOutput("value", signal).help("Synonym for `signal`");
}

void BaseSignal::reset() {
    signal = _prevSignal = initialSignal;
    flagIsUp = initialFlag;
    flagJustRaised = flagJustLowered = false;
    localReset();
}

void BaseSignal::update() {
    // Update flag
    bool oldFlag = flagIsUp;
    flagIsUp = computeFlag();
    flagJustRaised = !oldFlag && flagIsUp;
    flagJustLowered = oldFlag && !flagIsUp;

    // Update signal
    signal = computeSignal(flagIsUp);
    _prevSignal = signal;

}

} //namespace

