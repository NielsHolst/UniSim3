/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "dew_point.h"

using namespace base;
using namespace phys_math;

namespace boxes {

PUBLISH(DewPoint)

DewPoint::DewPoint(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes dew point");
    Input(temperature).unit("oC").help("Air temperature");
    Input(rh).unit("%").help("Air relative humidity");
    Output(value).unit("oC").help("Dew point temperature");
}

void DewPoint::reset() {
    update();
}

void DewPoint::update() {
    value = Tdew(temperature, rh);
}

} //namespace

