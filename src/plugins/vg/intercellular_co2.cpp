/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include "intercellular_co2.h"

using namespace base;

namespace vg {
	
PUBLISH(IntercellularCo2)

IntercellularCo2::IntercellularCo2(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes intercellular CO2 concentration");

    Input(boundaryLayerResistance).imports("crop/boundaryLayerResistance[rCo2]",CA);
    Input(stomatalResistance).imports("../stomatalResistance[rCo2]",CA);
    Input(indoorsCo2).imports("indoors/co2[value]",CA);
    Input(leafPhotosynthesis).imports("../leafPhotosynthesis[An]",CA); // μmol CO2 / leaf m2 / s
    Output(value).help("Intercellular CO2 concentration").unit("ppm");
    NamedOutput("ci", value).help("Intercellular CO2 concentration").unit("ppm");
}

void IntercellularCo2::reset() {
    value = indoorsCo2;
}

void IntercellularCo2::update() {
    double rTotal = boundaryLayerResistance*stomatalResistance/     // Ohm's Law
                   (boundaryLayerResistance+stomatalResistance),
           ci = indoorsCo2 - leafPhotosynthesis*rTotal;
    value = (ci>0.) ? ci : 0.;
}

} //namespace

