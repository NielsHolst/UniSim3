/* Copyright 2019-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Teis Boderskov, Aarhus University [tebo atbios.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "allocation.h"

using namespace base;
using std::max;
using std::min;

namespace saccharina {

PUBLISH(Allocation)

Allocation::Allocation(QString name, Box *parent)
    : Box(name, parent)
{
    help("allocates supplies according to demands");
    Input(dwStructure).unit("g dw/m").help("Structural biomass");
    Input(dwC).unit("g C/m").help("Biomass of carbon store");
    Input(dwN).unit("g N/m").help("Biomass of nitrogen store");
    Input(dwP).unit("g P/m").help("Biomass of phosphorus store");

    Input(UC).unit("g C/m").help("Uptaken supply of carbon");
    Input(UN).unit("g N/m").help("Uptaken supply of nitrogen");
    Input(UP).unit("g P/m").help("Uptaken supply of phosphorus");
    Input(CC).unit("g C/m").help("Carbon store content");
    Input(NN).unit("g N/m").help("Nitrogen store content");
    Input(PP).unit("g P/m").help("Nitrogen store content");

    Input(DMC).unit("g C/m").help("Maintenance demand for carbon");
    Input(DEC).unit("g C/m").help("Exudation demand for carbon");
    Input(DEN).unit("g N/m").help("Exudation demand for nitrogen");
    Input(DEP).unit("g P/m").help("Exudation demand for phosphorus");
    Input(DSC).unit("g C/m").help("Structural demand for carbon");
    Input(DSN).unit("g N/m").help("Structural demand for nitrogen");
    Input(DCC).unit("g C/m").help("Carbon store demand");
    Input(DNN).unit("g N/m").help("Nitrogen store demand");
    Input(DPP).unit("g P/m").help("Phosphorus store demand");

    Input(lambdaS).imports("cost[lambdaS]");
    Input(lambdaC).imports("cost[lambdaC]");
    Input(lambdaN).imports("cost[lambdaN]");
    Input(concSC).imports("biomass/structure[concC]");
    Input(concSN).imports("biomass/structure[concN]");
    Input(concCC).imports("biomass/storeC[concC]");
    Input(concNN).imports("biomass/storeN[concN]");
    Input(concPP).imports("biomass/storeP[concP]");
    Input(idealStructure).imports("proportions[idealStructure]");
    Input(idealStoreC).imports("proportions[idealStoreC]");
    Input(idealStoreN).imports("proportions[idealStoreN]");
    Input(idealStoreP).imports("proportions[idealStoreP]");

    Output(takenStoreC1).unit("g dw/m").help("Carbon store used in step 1");
    Output(takenStructure1).unit("g dw/m").help("Structure used in step 1");
    Output(negativeGrowth).unit("bool").help("Was structure used for maintenance?");

    Output(SEC).unit("g C/m").help("Exuded carbon");
    Output(SEN).unit("g N/m").help("Exuded nitrogen");
    Output(SEP).unit("g P/m").help("Exuded phosphorus");
    Output(sdExudationC).unit("[0;1]").help("Supply/demand ratio for carbon exudation");
    Output(sdExudationN).unit("[0;1]").help("Supply/demand ratio for nitrogen exudation");
    Output(sdExudationP).unit("[0;1]").help("Supply/demand ratio for phosphorus exudation");
    Output(takenStoreC2).unit("g dw/m").help("Carbon store used in step 2");
    Output(takenStoreN2).unit("g dw/m").help("Nitrogen store used in step 2");
    Output(takenStoreP2).unit("g dw/m").help("Phosphorus store used in step 2");

    Output(SSC).unit("g C/m").help("Carbon supply for structural growth");
    Output(SSN).unit("g N/m").help("Nitrogen supply for structural growth");
    Output(supplyStructure3).unit("g dw/m").help("Biomass supply for structural growth");
    Output(sdStructure).unit("[0;1]").help("Supply/demand ratio for structural growth");
    Output(takenStoreC3).unit("g dw/m").help("Carbon store used in step 3");
    Output(takenStoreN3).unit("g dw/m").help("Nitrogen store used in step 3");

    Output(SNN).unit("g N/m").help("Nitrogen supply for the nitrogen store");
    Output(supplyStoreN4).unit("g dw/m").help("Biomass supply for nitrogen store");
    Output(sdStoreN).unit("[0;1]").help("Supply/demand ratio for the nitrogen store");
    Output(takenStoreC4).unit("g dw/m").help("Carbon store used in step 4");

    Output(SCC).unit("g C/m").help("Carbon supply for the carbon store");
    Output(supplyStoreC5).unit("g dw/m").help("Biomass supply for carbon store");
    Output(sdStoreC).unit("[0;1]").help("Supply/demand ratio for the carbon store");

    Output(SPP).unit("g P/m").help("Phosphorus supply for the phosphorus store");
    Output(supplyStoreP6).unit("g dw/m").help("Biomass supply for phosphorus store");
    Output(sdStoreP).unit("[0;1]").help("Supply/demand ratio for the phosphorus store");

    Output(supplyStructure).unit("g dw/m").help("Biomass supply allocated to structure");
    Output(supplyStoreC).unit("g dw/m").help("Biomass supply allocated to carbon store");
    Output(supplyStoreN).unit("g dw/m").help("Biomass supply allocated to nitrogen store");
    Output(supplyStoreP).unit("g dw/m").help("Biomass supply allocated to phosphorus store");
    Output(buildingCost).unit("g C/m").help("Carbon spent on building costs");

    // Populate supplies
    supplies = {
        {Source::UptakenC, &UC},
        {Source::UptakenN, &UN},
        {Source::UptakenP, &UP},
        {Source::StoreCC,  &CC},
        {Source::StoreNN,  &NN},
        {Source::StorePP,  &PP}
    };
}

inline double sd(double supply, double demand) {
    return (demand > 0.) ? supply/demand : 0.;
}

void Allocation::reset() {
    update();
}

void Allocation::update() {
    // Allocate maintenance respiration
    maintenance();
    if (negativeGrowth) {
        supplyStructure3 =
        takenStoreC2 = takenStoreC3 = takenStoreC4 = supplyStoreC5 =
        takenStoreN2 = takenStoreN3 = supplyStoreN4 =
        takenStoreP2 = supplyStoreP6 = 0.;
    }
    // Allocate rest
    else {
        exudation();
        structuralGrowth();
        storeN();
        storeC();
        storeP();
    }
    // Roundup
    supplyStructure = takenStructure1 + supplyStructure3;
    supplyStoreC = takenStoreC1 + takenStoreC2 + takenStoreC3 + takenStoreC4 + supplyStoreC5;
    supplyStoreN = takenStoreN2 + takenStoreN3 + supplyStoreN4;
    supplyStoreP = takenStoreP2 + supplyStoreP6;
    buildingCost = lambdaS*supplyStructure3 + lambdaC*supplyStoreC5 + lambdaN*supplyStoreN4;
}

void Allocation::maintenance() {
    double
        CCbefore = CC,
        supplyC = take(DMC, {Source::UptakenC, Source::StoreCC});
    takenStoreC1 = (CC - CCbefore)/concCC;
    negativeGrowth = (supplyC < DMC);
    takenStructure1 = negativeGrowth ? (supplyC - DMC)/concSC : 0;
}

void Allocation::exudation() {
    double
        CCbefore = CC,
        NNbefore = NN,
        PPbefore = PP;
    SEC = take(DEC, {Source::UptakenC, Source::StoreCC}),
    SEN = take(DEN, {Source::UptakenN, Source::StoreNN}),
    SEP = take(DEP, {Source::UptakenP, Source::StorePP});
    takenStoreC2 = (CC - CCbefore)/concCC;
    takenStoreN2 = (NN - NNbefore)/concNN;
    takenStoreP2 = (PP - PPbefore)/concPP;
    sdExudationC = sd(SEC, DEC);
    sdExudationN = sd(SEN, DEN);
    sdExudationP = sd(SEP, DEP);
}

void Allocation::structuralGrowth() {
    double
        CCbefore = CC,
        NNbefore = NN,
        withCost = 1. + lambdaS/concSC,
        supplyC0 = askFor(DSC*withCost, {Source::UptakenC, Source::StoreCC})/withCost,
        supplyN0 = askFor(DSN, {Source::UptakenN, Source::StoreNN}),
        supplyC0dw = supplyC0/concSC,
        supplyN0dw = supplyN0/concSN;
    switch (limitation(DSC, DSN, supplyC0dw, supplyN0dw)) {
    case Limitation::Climited:
        SSC = take(DSC*withCost, {Source::UptakenC, Source::StoreCC})/withCost;
        SSN = take(SSC*concSN/concSC, {Source::UptakenN, Source::StoreNN});
        break;
    case Limitation::Nlimited:
        SSN = take(DSN, {Source::UptakenN, Source::StoreNN});
        SSC = take(SSN*concSC/concSN*withCost, {Source::UptakenC, Source::StoreCC})/withCost;
        break;
    case Limitation::NoDemands:
        SSC = SSN = 0.;
    }
    supplyStructure3 = SSC/concSC;
    takenStoreC3 = (CC - CCbefore)/concCC;
    takenStoreN3 = (NN - NNbefore)/concNN;
    sdStructure = sd(SSC,DSC);
}

void Allocation::storeN() {
    double
        CCbefore = CC,
        Ccost    = lambdaN/concNN, // gC/gN
        supplyN0 = askFor(DNN, {Source::UptakenN}), // gN
        supplyN1 = askFor(DNN*Ccost, {Source::UptakenC, Source::StoreCC})/Ccost, // gC/(gC/gN) = gN
        supplyN, supplyC;
    Limitation limitation = (DNN == 0.)        ? Limitation::NoDemands :
                            (supplyN0 < supplyN1) ? Limitation::Nlimited :
                                                    Limitation::Climited;
    switch (limitation) {
    case Limitation::Nlimited:
        supplyN = take(DNN, {Source::UptakenN}); // gN
        supplyC = take(supplyN*Ccost, {Source::UptakenC, Source::StoreCC}); // gN*gC/gN = gC
        break;
    case Limitation::Climited:
        supplyC = take(DNN*Ccost, {Source::UptakenC, Source::StoreCC}); // gN*gC/gN = gC
        supplyN = take(supplyC/Ccost, {Source::UptakenN}); // gC/(gC/gN) = gN
        break;
    case Limitation::NoDemands:
        supplyC = supplyN = 0.;
    }
    SNN = supplyN;
    supplyStoreN4 = SNN/concNN;
    takenStoreC4 = (CC - CCbefore)/concCC;
    sdStoreN = sd(SNN, DNN);
}

void Allocation::storeC() {
    double withCost = 1. + lambdaC/concCC;
    SCC = take(DCC*withCost, {Source::UptakenC})/withCost;
    supplyStoreC5 = SCC/concCC;
    sdStoreN = sd(SCC, DCC);
}

void Allocation::storeP() {
    SPP = take(DPP, {Source::UptakenP});
    supplyStoreP6 = SPP/concPP;
    sdStoreP = sd(SPP, DPP);
}

double Allocation::take(double demand, Sources sources) {
    return askFor(demand, sources, true);
}

double Allocation::askFor(double demand, Sources sources, bool doTake) const {
    double supply = 0;
    // Go through sources in order of priority
    for (auto it=sources.constBegin(); it!=sources.constEnd(); ++it) {
      double &source(*supplies.value(*it));
      double lacking = demand - supply,
             taken = (lacking > source) ? source : lacking;
      supply = supply + taken;
      if (doTake)
          source -= taken;
      // Stop if supply meets demand
      if (abs(supply - demand) < 1e-6) break;
    }
    return supply;
}

Allocation::Limitation Allocation::limitation(double demandC, double demandN, double supplyCdw, double supplyNdw) const {
    Limitation limitation;
    if (demandC > 0.) {
        if (demandN > 0.)
            limitation = (supplyCdw < supplyNdw) ? Limitation::Climited : Limitation::Nlimited;
        else
            limitation = Limitation::Climited;
    }
    else {
        limitation = (demandN > 0.) ? Limitation::Nlimited : Limitation::NoDemands;
    }
    return limitation;
}

}
