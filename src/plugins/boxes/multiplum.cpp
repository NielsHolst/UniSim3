/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "multiplum.h"

using namespace base;

namespace boxes {

PUBLISH(Multiplum)

Multiplum::Multiplum(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes multiplum of factors");
    Input(factors).help("Input factors");
    Output(value).help("The multiplum");
}

void Multiplum::reset() {
    update();
}

void Multiplum::update() {
    value = 1.;
    for (double factor : factors)
        value *= factor;
}

}
