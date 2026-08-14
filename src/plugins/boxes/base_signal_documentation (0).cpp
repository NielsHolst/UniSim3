/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "base_signal_documentation.h"

using namespace base;

namespace boxes {

PUBLISH(BaseSignalDocumentation)

BaseSignalDocumentation::BaseSignalDocumentation(QString name, Box *parent)
    : BaseSignal(name, parent)
{
    help("is a base class for setting signals").documentClassName("BaseSignal");
}

bool BaseSignalDocumentation::computeFlag() {
    return false;
}

double BaseSignalDocumentation::computeSignal(bool) {
    return 0.;
}


} //namespace

