/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "ah_from_rh.h"

using namespace base;

namespace vg {

PUBLISH(AhFromRh)

AhFromRh::AhFromRh(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes absolute humidity from relative humidity");
    Input(temperature).help("Temperature").unit("oC");
    Input(rh).help("Relative humidity").unit("[0;100]");
    Output(value).help("Absolute humidity").unit("kg/m3");
}

void AhFromRh::reset() {
    update();
}

void AhFromRh::update() {
    value = phys_math::ahFromRh(temperature, rh);
}

} //namespace

