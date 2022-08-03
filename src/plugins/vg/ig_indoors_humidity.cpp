/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "ig_indoors_humidity.h"
#include <base/phys_math.h>
#include <base/publish.h>

using namespace phys_math;
using namespace base;

namespace vg {
	
PUBLISH(IgIndoorsHumidity)

IgIndoorsHumidity::IgIndoorsHumidity(QString name, Box *parent)
	: Box(name, parent)
{
    help("models indoors humidity (simplified for IG)");
    Input(rh).help("Indoors relative humidity").unit("[0;100]");
    Input(temperature).imports("indoors/temperature[value]",CA).unit("oC");
    Output(ah).help("Indoors absolute humidity").unit("kg/m3");
}

void IgIndoorsHumidity::reset() {
    update();
}

void IgIndoorsHumidity::update() {
    ah = ahFromRh(temperature, rh);
}

} //namespace
