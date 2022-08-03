/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "fixed_signal.h"

using namespace base;

namespace boxes {

PUBLISH(FixedSignal)

FixedSignal::FixedSignal(QString name, Box *parent)
    : BaseSignal(name, parent){
    help("fixes a signal at the initial value");
}

bool FixedSignal::computeFlag() {
    return initialFlag;
}

double FixedSignal::computeSignal(bool) {
    return initialSignal;
}


} //namespace

