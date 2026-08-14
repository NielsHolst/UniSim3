/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "inv_log10.h"

using namespace base;

namespace eq {

PUBLISH(InvLog10)

InvLog10::InvLog10(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes 10^x");
    Input(x).equals(1).help("Input value");
    Output(value).help("Output value");
}

void InvLog10::reset() {
    update();
}

void InvLog10::update() {
    value = pow(10., x);
}

} //namespace

