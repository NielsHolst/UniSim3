/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "demand_nitrogen_erosion.h"

using namespace base;

namespace saccharina {

PUBLISH(DemandNitrogenErosion)

DemandNitrogenErosion::DemandNitrogenErosion(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates carbon loss by erosion");
    Input(areaErosion).imports("area/erosion[value]");
    Input(proportionN).imports("nitrogen/structure[proportionC]");
    Input(kA).imports("area[kA]");
    Output(value).unit("g N").help("Nitrogen lost");
}

void DemandNitrogenErosion::reset() {
   update();
}

void DemandNitrogenErosion::update() {
    value = proportionN*kA*areaErosion;
}

}
