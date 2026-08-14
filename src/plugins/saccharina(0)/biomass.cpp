/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "biomass.h"

using namespace base;

namespace saccharina {

PUBLISH(Biomass)

Biomass::Biomass(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the yield, weight, C and N percent of the system");
    Input(structuralMass).imports("structure[mass]");
    Input(structuralMassGrowth).imports("allocation[supplyMassStructure]");
    Input(plantDensity).imports("area[plantDensity]");
    Input(Cstruct).equals(0.21).unit("g C/g structure").help("Amount of structurally bound carbon");
    Input(Nstruct).equals(0.015).unit("g N/g structure").help("Amount of structurally bound nitrogen");
    Input(kdw).equals(0.0785).help("Dry weight to wet weight ratio for structural mass");
    Input(kN).equals(2.72).help("Mass of nitrogen reserves per gram nitrogen");
    Input(kC).equals(2.12).help("Mass of carbon reserves per gram carbon");
    Input(C).imports("reserves/carbon[proportion]");
    Input(N).imports("reserves/nitrogen[proportion]");
    Input(date).imports("calendar[date]");
    Output(dryWeight).unit("g DW").help("Dry weight per plant");
    Output(dryWeightGrowth).unit("g DW/h").help("Dry weight growth increment");
    Output(wetWeight).unit("g WW").help("Wet weight per plant");
    Output(carbonWeight).unit("g C").help("Carbon total weight");
    Output(nitrogenWeight).unit("g N").help("Nitrogen total weight");
    Output(carbonPct).unit("%").help("Percentage carbon content (by dry weight)");
    Output(nitrogenPct).unit("%").help("Percentage nitrogen content (by dry weight)");
    Output(CNratio).help("C/N ratio");
    Output(dryWeightYield).unit("g DW per m rope").help("Standing yield (dry weight)");
    Output(wetWeightYield).unit("g WW per m rope").help("Standing yield (wet weight)");
    Output(growthRatePct).unit("% per d").help("Percentage growth in dry weight");
    Output(nitrogenUptakeRate).unit("g N per g DW per d").help("Relative uptake rate of nitrogen");
}

void Biomass::reset() {
    curDate = date;
    update();
    previousDryWeight = dryWeight;
    previousNitrogenWeight = nitrogenWeight;
}

void Biomass::update() {
   dryWeight       = structuralMass * (1 + C*kC + N*kN) ;
   carbonWeight    = structuralMass * (C + Cstruct);
   nitrogenWeight  = structuralMass * (N + Nstruct);
   dryWeightGrowth = structuralMassGrowth * (1 + C*kC + N*kN);

   carbonPct   = (dryWeight>0.)      ? 100.*(carbonWeight/dryWeight)   : 0.;
   nitrogenPct = (dryWeight>0.)      ? 100.*(nitrogenWeight/dryWeight) : 0.;
   CNratio     = (nitrogenWeight>0.) ? carbonWeight/nitrogenWeight     : 0.;

   wetWeight = dryWeight/kdw;

   dryWeightYield = dryWeight*plantDensity;
   wetWeightYield = wetWeight*plantDensity;

   if (date > curDate) {
       curDate = date;
       // Change in dry weight relattive to dry weight
       double dW = dryWeight - previousDryWeight;
       growthRatePct = (previousDryWeight==0.) ? 0. : 100.*dW/previousDryWeight;
       previousDryWeight = dryWeight;
       // Change in nitrogen weight relative to dry(!) weight
       double dN = nitrogenWeight - previousNitrogenWeight;
       nitrogenUptakeRate = (previousDryWeight==0.) ? 0. : dN/previousDryWeight;
       previousNitrogenWeight = nitrogenWeight;
   }
}


}

