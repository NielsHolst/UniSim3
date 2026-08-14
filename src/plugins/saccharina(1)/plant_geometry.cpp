/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <cmath>
#include <base/publish.h>
#include "plant_geometry.h"

using namespace base;

namespace saccharina {

PUBLISH(PlantGeometry)

PlantGeometry::PlantGeometry(QString name, Box *parent)
    : Box(name, parent)
{
    help("models canopy dimensions");
    Input(biomassStruct).imports("plant[biomassStruct]");
    Input(biomassTotal).imports("plant[biomassTotal]");
    Input(a).equals(0.330).unit("m2/g dw").help("Area-Wstruct coefficient");
    Input(b).equals(0.629).unit("-").help("Area-Wstruct exponent");
    Input(c).equals(0.123).unit("m").help("Length-area coefficient");
    Input(d).equals(0.709).unit("-").help("Length-area exponent");
    Input(relWidth).equals(0.9).unit("-").help("Relative canopy width");
    Output(area).unit("m2/m").help("Frond (\"leaf\") area");
    Output(length).unit("m").help("Plant length");
    Output(crownZoneArea).unit("m2/m").help("Crown zone area");
    Output(lai).unit("m2/m2").help("Leaf area index                                                                                                                                                                                                             ");
}

void PlantGeometry::reset() {
    update();
}

void PlantGeometry::update() {
    area = a*pow(biomassStruct, b);
    length = c*pow(area, d);
    crownZoneArea = relWidth*length;
    lai = (crownZoneArea > 0.) ? area/crownZoneArea : 0.;
}


}
