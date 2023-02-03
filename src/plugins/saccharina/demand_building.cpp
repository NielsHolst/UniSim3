/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "demand_building.h"

using namespace base;

namespace saccharina {

PUBLISH(DemandBuilding)

DemandBuilding::DemandBuilding(QString name, Box *parent)
    : Density(name, parent)
{
    help("computes building cost");
    Input(demandStructure).unit("g dw/m").help("Demand of structural tissue");
    Input(demandStoreC).unit("g C/m").help("Demand of carbon store");
    Input(demandStoreN).unit("g N/m").help("Demand of nitrogen store");
    Input(lambdaS).unit("g C/g dw").help("Cost of building structural tissue");
    Input(lambdaC).unit("g C/g dw").help("Cost of building carbon store");
    Input(lambdaN).unit("g C/g dw").help("Cost of building nitrogen store");
    Input(concStoreC).imports("proportions[concStoreC]");
    Input(constStoreN).imports("proportions[concStoreN]");
}

void DemandBuilding::reset() {
   update();
}

void DemandBuilding::update() {
    dw = N = 0;
    C = lambdaS*demandStructure + lambdaC*demandStoreC/concStoreC + lambdaN*demandStoreN/constStoreN;
}

}
