/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "log10.h"

using namespace base;

namespace eq {

PUBLISH(Log10)

Log10::Log10(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes log10(x)");
    Input(x).equals(1).help("Input value");
    Output(value).help("Output value");
}

void Log10::reset() {
    update();
}

void Log10::update() {
    Q_ASSERT(x>0);
    value = log10(x);
}

} //namespace

