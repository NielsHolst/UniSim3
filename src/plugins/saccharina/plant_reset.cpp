/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "plant_reset.h"

using namespace base;

namespace saccharina {

PUBLISH(PlantReset)

PlantReset::PlantReset(QString name, Box *parent)
    : Box(name, parent)
{
    help("resets plant biomass and reserves");
    Input(biomassInit).equals(0.1).unit("g dw/m").help("Initial total biomass");
    Input(concCInit).equals(0.3).unit("g C/g dw").help("Initial total biomass carbon concentration");
    Input(concNInit).equals(0.02).unit("g N/g dw").help("Initial total biomass nitrogen concentration");
    Input(propCStruct).imports("proportions[propCStruct]").unit("g C/g dw").help("Proportion of carbon in the structural biomass");
    Input(propNStruct).imports("proportions[propNStruct]").unit("g N/g dw").help("Proportion of nitrogen in the structural biomass");
    Input(propCRes).imports("proportions[propCRes]").unit("g C/g dw").help("Proportion of carbon in the carbon reserve");
    Input(propNRes).imports("proportions[propNRes]").help("Proportion of nitrogen in the nitrogen reserve");
    Output(biomassStruct).unit("g dw/m").help("Initial structural biomass");
    Output(biomassCRes).unit("g dw/m").help("Initial carbon reserves biomass");
    Output(biomassNRes).unit("g dw/m").help("Initial nitrogen reserves biomass");
}

void PlantReset::reset() {
    double k = propCStruct*propNRes + propCRes*(propNStruct-propNRes);
    biomassCRes = biomassInit*(propCStruct*(propNRes-concNInit) + concCInit*(propNStruct-propNRes))/k;
    biomassNRes = biomassInit*(propNStruct*(propCRes-concCInit) + concNInit*(propCStruct-propCRes))/k;
    biomassStruct = biomassInit - biomassCRes - biomassNRes;
}


}
