/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "demand_carbon_erosion.h"

using namespace base;

namespace saccharina {

PUBLISH(DemandCarbonErosion)

DemandCarbonErosion::DemandCarbonErosion(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates carbon loss by erosion");
    Input(areaErosion).imports("area/erosion[value]");
    Input(proportionC).imports("carbon/structure[proportionC]");
    Input(kA).imports("area[kA]");
    Output(value).unit("g C").help("Carbon lost");
}

void DemandCarbonErosion::reset() {
   value = 0.;
}

void DemandCarbonErosion::update() {
    value = proportionC*kA*areaErosion;
}

}
