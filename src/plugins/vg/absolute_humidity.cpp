/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "absolute_humidity.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(AbsoluteHumidity)

AbsoluteHumidity::AbsoluteHumidity(QString name, Box *parent)
    : Box(name, parent)
{
    help("converts relative humidity to absolute humidity");
    Input(rh).unit("%").help("Relative humidity");
    Input(temperature).unit("oC").help("Temperature");
    Output(value).unit("kg/m3").help("Absolute humidity");
}

void AbsoluteHumidity::reset() {
    update();
}

void AbsoluteHumidity::update(){
    value = ahFromRh(temperature, rh);
}

} //namespace


