/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "plant.h"

using namespace base;

namespace saccharina {

PUBLISH(Plant)

Plant::Plant(QString name, Box *parent)
    : Box(name, parent)
{
    help("keeps tracks of plant biomass and reserves");
    Input(optCConc).equals(0.35).unit("g C/g dw").help("Optimum total biomass carbon concentration");
    Input(optNConc).equals(0.05).unit("g C/g dw").help("Optimum total biomass nitrogen concentration");
    Input(propCStruct).imports("proportions[propCStruct]").unit("g C/g dw").help("Proportion of carbon in the structural biomass");
    Input(propNStruct).imports("proportions[propNStruct]").unit("g N/g dw").help("Proportion of nitrogen in the structural biomass");
    Input(propCRes).imports("proportions[propCRes]").unit("g C/g dw").help("Proportion of carbon in the carbon reserve");
    Input(propNRes).imports("proportions[propNRes]").unit("g N/g dw").help("Proportion of nitrogen in the nitrogen reserve");
    Input(initBiomassStruct).unit("g dw/m").imports("plantReset[biomassStruct]");
    Input(initBiomassCRes).unit("g dw/m").imports("plantReset[biomassCRes]");
    Input(initBiomassNRes).unit("g dw/m").imports("plantReset[biomassNRes]");
    Input(biomassStruct).unit("g dw/m").help("Structural biomass").imports("biomassStruct[content]");
    Input(supplyReservesC).unit("g C/m").imports("allocation[supplyReservesC]");
    Input(supplyReservesN).unit("g N/m").imports("allocation[supplyReservesN]");

    Output(biomassCRes).unit("g dw/m").help("Carbon reserves biomass");
    Output(biomassNRes).unit("g dw/m").help("Nitrogen reserves biomass");
    Output(biomassTotal).unit("g dw/m").help("Total biomass");
    Output(massCStruct).unit("g C/m").help("Mass of carbon in the structural biomass");
    Output(massNStruct).unit("g N/m").help("Mass of nitrogen in the structural biomass");
    Output(massCRes).unit("g C/m").help("Mass of carbon in the carbon reserve");
    Output(massNRes).unit("g N/m").help("Mass of nitrogen in the carbon reserve");
    Output(concC).unit("g C/g dw").help("Total biomass carbon concentration");
    Output(concN).unit("g N/g dw").help("Total biomass nitrogen concentration");
}

void Plant::reset() {
    biomassStruct = initBiomassStruct;
    biomassCRes = initBiomassCRes;
    biomassNRes = initBiomassNRes;
    update();
}

void Plant::update() {
    biomassCRes += supplyReservesC/propCRes;
    biomassNRes += supplyReservesN/propNRes;
    biomassTotal = biomassStruct + biomassCRes + biomassNRes;
    massCStruct = propCStruct*biomassStruct;
    massNStruct = propNStruct*biomassStruct;
    massCRes = propCRes*biomassCRes;
    massNRes = propNRes*biomassNRes;
    concC = (massCStruct + massCRes)/biomassTotal;
    concN = (massNStruct + massNRes)/biomassTotal;
}

}
