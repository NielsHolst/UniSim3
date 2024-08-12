/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "sah.h"

using namespace base;

namespace boxes {

PUBLISH(Sah)

Sah::Sah(QString name, Box *parent)
    : Box(name, parent) {
    help("computes saturated absolute humidity");
    Input(temperature).unit("oC").help("Temperature");
    Input(value).unit("kg/m3").help("Saturated absolute humidity");
}

void Sah::reset() {
    update();
}

void Sah::update() {
    value = phys_math::sah(temperature);
}

} //namespace

