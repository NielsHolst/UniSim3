/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "demand_nitrogen_structure.h"

using namespace base;

namespace saccharina {

PUBLISH(DemandNitrogenStructure)

DemandNitrogenStructure::DemandNitrogenStructure(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates demand for structural nitrogen");
    Input(demandCarbonStructure).imports("demand/carbon/structure[value]");
    Input(proportionC).imports("/sim/structure[proportionC]");
    Input(proportionN).imports("/sim/structure[proportionN]");
    Output(value).unit("g N").help("Nitrogen demand for structural growth");
}

void DemandNitrogenStructure::reset() {
   update();
}

void DemandNitrogenStructure::update() {
    value = proportionN*demandCarbonStructure/proportionC;
}

}
