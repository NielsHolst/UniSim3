/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "topsoil_in_soil.h"
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (TopsoilInSoil)
	
TopsoilInSoil::TopsoilInSoil(QString name, Box *parent)
	: Box(name, parent)
{

    help("manages fraction of pesticide reaching soil air, water, and solids");
    Input(Z).unit("mol/m3/Pa").equals(0.).help("Z-value of pesticide in air, water, and solid phase");
    Input(V).unit("m3").equals(0.).help("Volume of air, water, and solids in the soil volume susceptible for volatilization");
    Input(VZ).equals(0.).help("Sum of VZ-value of all three phase i.e. air, water, and solid phases");

    Output(fr).unit("fraction").help("Fraction of pesticide reaching soil air, water, and solids");
}

void TopsoilInSoil::reset() {
    update();
}


void TopsoilInSoil::update() {
    fr = (VZ > 0) ? V*Z/VZ : 0.;
}

} //namespace

