/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "allocation.h"

using namespace base;
using phys_math::minmax;

namespace saccharina {

PUBLISH(Allocation)

Allocation::Allocation(QString name, QObject *parent)
    : Box(name, parent)
{
    help("allocates C and N supplies");
    Input(structuralProportionC).imports("/sim/structure[proportionC]");
    Input(structuralProportionN).imports("/sim/structure[proportionN]");
    Input(structuralMass).imports("structure[mass]");
    Input(curReservesProportionC).imports("reserves/carbon[proportion]");
    Input(curReservesProportionN).imports("reserves/nitrogen[proportion]");
    Input(reservesMaxProportionC).imports("reserves/carbon[maxProportion]");
    Input(reservesMaxProportionN).imports("reserves/nitrogen[maxProportion]");
    Input(supplyCarbonTotal).imports("photosynthesis[supply]");
    Input(supplyNitrogenTotal).imports("nitrogenUptake[supply]");
    Input(demandCarbonRespiration).imports("demand/carbon/respiration[value]");
    Input(demandCarbonStructure).imports("demand/carbon/structure[value]");
    Input(demandNitrogenStructure).imports("demand/nitrogen/structure[value]");
    Input(proportionExuded).imports("demand/carbon/exudation[proportion]");
    Input(demandCarbonErosion).imports("demand/carbon/erosion[value]");
    Input(demandNitrogenErosion).imports("demand/nitrogen/erosion[value]");

    Output(MC0).unit("g C").help("Metabolic carbon pool step 0");
    Output(MC1).unit("g C").help("Metabolic carbon pool step 1");
    Output(MC2).unit("g C").help("Metabolic carbon pool step 2");
    Output(MC3).unit("g C").help("Metabolic carbon pool step 3");
    Output(MN0).unit("g N").help("Metabolic nitrogen pool step 0");
    Output(MN1).unit("g N").help("Metabolic nitrogen pool step 1");
    Output(supplyCarbonStructure).unit("g C").help("Carbon supply to structural growth");
    Output(supplyCarbonExudation).unit("g C").help("Carbon supply to exudation");
    Output(supplyCarbonReserves).unit("g C").help("Carbon supply to reserves");
    Output(structuralMassGrowth).unit("g structure").help("Growth in structural mass");
    Output(structuralMassLoss).unit("g structure").help("Loss of structural mass to excess respiration");
    Output(reservesProportionC).unit("[0;1]").help("New proportion of carbon reserves");
    Output(reservesProportionN).unit("[0;1]").help("New proportion of nitrogen reserves");
    Output(sdCarbon).unit("[0;1]").help("Supply/demand ratio of carbon for structural growth");
    Output(sdNitrogen).unit("[0;1]").help("Supply/demand ratio of nitrogen for structural growth");
}

void Allocation::reset() {
    update();
}

inline double min(double a, double b, double c) {
    return std::min(std::min(a,b), c);
}

void Allocation::update() {
    double carbonReservesBefore = curReservesProportionC*structuralMass,
           carbonReservesAfter;
    MC0 = curReservesProportionC*structuralMass + supplyCarbonTotal;
    MC1 = MC0 - demandCarbonRespiration;
    if (MC1 < 0.) {
        structuralMassGrowth = MC1/structuralProportionC;
        structuralMassLoss = -structuralMassGrowth;
        sdCarbon = sdNitrogen =
        supplyCarbonStructure =
        supplyCarbonExudation =
        MN0 = MN1 = MC2 = MC3 =
        reservesProportionC = 0.;
        reservesProportionN = curReservesProportionN;
        carbonReservesAfter = 0.;
    }
    else {
        structuralMassLoss = 0.;
        MN0 = curReservesProportionN*structuralMass + supplyNitrogenTotal;
        sdCarbon   = (demandCarbonStructure==0.) ? 0. : MC1/(1. + proportionExuded)/demandCarbonStructure;
        sdNitrogen = (demandNitrogenStructure==0.) ? 0. : MN0/demandNitrogenStructure;
        supplyCarbonStructure = min(sdCarbon, sdNitrogen, 1.)*demandCarbonStructure;
        MC2 = MC1 - (1.+proportionExuded)*supplyCarbonStructure;
        structuralMassGrowth = (supplyCarbonStructure - demandCarbonErosion)/structuralProportionC;
        MC3 = MC2 - proportionExuded*structuralProportionC*structuralMass;
        supplyCarbonExudation = proportionExuded*(structuralProportionC*structuralMass + supplyCarbonStructure);
        // Update reserves
        double newStructuralMass = structuralMass + structuralMassGrowth;
        reservesProportionC = minmax(0., MC3/newStructuralMass, reservesMaxProportionC);
        MN1 = MN0 - structuralMassGrowth*structuralProportionN - demandNitrogenErosion;
        reservesProportionN = minmax(0., MN1/newStructuralMass, reservesMaxProportionN);
        carbonReservesAfter = std::min(reservesProportionC*newStructuralMass, reservesMaxProportionC);
    }
    supplyCarbonReserves = carbonReservesAfter - carbonReservesBefore;
}

}
