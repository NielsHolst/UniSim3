/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "ah_from_rh.h"

using namespace base;

namespace boxes {

PUBLISH(AhFromRh)

AhFromRh::AhFromRh(QString name, Box *parent)
    : Box(name, parent) {
    help("computes absolute humidity from relative humidity");
    Input(temperature).unit("oC").help("Temperature");
    Input(rh).unit("[0;100]").help("Relative humidity");
    Input(value).unit("kg/m3").help("Absolute humidity");
}

void AhFromRh::reset() {
    update();
}

void AhFromRh::update() {
    value = phys_math::ahFromRh(temperature, rh);
}

} //namespace

