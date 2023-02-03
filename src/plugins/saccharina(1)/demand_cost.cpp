/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "demand_cost.h"

using namespace base;

namespace saccharina {

PUBLISH(DemandCost)

DemandCost::DemandCost(QString name, Box *parent)
    : Box(name, parent)
{
    help("calculates demand for construction cost");
    Input(costStruct).equals(0.571).unit("g C/g C").help("Cost for construction of structure");
    Input(costCRes).equals(0.267).unit("g C/g C").help("Cost for construction of carbon reserves");
    Input(costNRes).equals(0.781).unit("g C/g N").help("Cost for construction of nitrogen reserves");
    Input(demandGrowthC).imports("../growth[carbon]");
    Input(demandCRes).imports("../reserves[carbon]");
    Input(demandNRes).imports("../reserves[nitrogen]");
    Output(carbon).unit("g C/m").help("Carbon demand for construction cost");
}

void DemandCost::reset() {
   update();
}

void DemandCost::update() {
    carbon = costStruct*demandGrowthC + costCRes*demandCRes + costNRes*demandNRes;
}

}
