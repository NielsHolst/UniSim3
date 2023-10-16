/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "priority_signal.h"

using namespace base;

namespace boxes {

PUBLISH(PrioritySignal)

PrioritySignal::PrioritySignal(QString name, Box *parent)
    : BaseSignal(name, parent){
    help("selects signal among child signals");
    Input(myFlags).imports("./BaseSignal::*[flagIsUp]");
    Input(mySignals).imports("./BaseSignal::*[signal]");
    Input(reverseOrder).equals(false).help("Find first signal!=0 from top (false) or bottom (true)?").unit("y|n");
}

void PrioritySignal::localReset() {
    computeFlag();
    update();
}

bool PrioritySignal::computeFlag() {
    for (auto flag : myFlags) {
        if (flag)
            return true;
    }
    return false;
}

double PrioritySignal::computeSignal(bool flag) {
    if (!flag)
        return port("initialSignal")->value<double>();
    for (auto[flag,signal] = std::pair(myFlags.begin(), mySignals.begin());
         flag!=myFlags.end(); ++flag, ++signal) {
        if (*flag)
            return *signal;
    }
    return 0.;
}

} //namespace

