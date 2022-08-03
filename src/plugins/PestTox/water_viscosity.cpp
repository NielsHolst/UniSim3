/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "water_viscosity.h"
#include <cmath>
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (WaterViscosity)
	
WaterViscosity::WaterViscosity(QString name, Box *parent)
	: Box(name, parent)
{
    help("manages viscosity of water");
    Input(temperature).unit("Â°C").equals(25.).help("Daily soil temperature");

    Output(value).unit("centipoise").help("Viscosity of water");
}

void WaterViscosity::reset() {
    update();
}

void WaterViscosity::update() {
    double x = 1.337 + temperature*0.001705;
    double y = 1. + temperature*0.01849;
    value = pow(x/y, 2);
}

} //namespace

