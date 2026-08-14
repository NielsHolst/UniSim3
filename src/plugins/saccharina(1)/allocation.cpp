/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "allocation.h"

using namespace base;
using std::min;
using std::max;

namespace saccharina {

PUBLISH(Allocation)

Allocation::Allocation(QString name, Box *parent)
    : Box(name, parent)
{
    help("models allocation of carbon and nitrogen supplies");
    Input(massCStruct).imports("plant[massCStruct]");
    Input(massNStruct).imports("plant[massNStruct]");
    Input(massCRes).imports("plant[massCRes]");
    Input(massNRes).imports("plant[massNRes]");
    Input(propCStruct).imports("plant[propCStruct]");
    Input(propNStruct).imports("plant[propNStruct]");
    Input(propCRes).imports("plant[propCRes]");
    Input(propNRes).imports("plant[propNRes]");
    Input(optCConc).imports("plant[optCConc]");
    Input(optNConc).imports("plant[optNConc]");
    Input(supplyC).imports("supply/carbon[value]");
    Input(supplyN).imports("supply/nitrogen[value]");
    Input(demandRespirationC).imports("demand/respiration[carbon]");
    Input(demandGrowthC).imports("demand/growth[carbon]");
    Input(demandGrowthN).imports("demand/growth[nitrogen]");
    Input(demandReservesC).imports("demand/reserves[carbon]");
    Input(demandReservesN).imports("demand/reserves[nitrogen]");
    Input(demandCostC).imports("demand/cost[carbon]");
    Input(costStruct).imports("cost[costStruct]");
    Input(costCRes).imports("cost[costCRes]");
    Input(costNRes).imports("cost[costNRes]");

    Output(supplyRespirationC).unit("g C/m").help("Carbon supply for maintenance respiration");
    Output(supplyGrowthC).unit("g C/m").help("Carbon supply for structural growth");
    Output(supplyGrowthN).unit("g N/m").help("Nitrogen supply for structural growth");
    Output(supplyReservesC).unit("g C/m").help("Carbon supply for reserves");
    Output(supplyReservesN).unit("g N/m").help("Nitrogen supply for reserves");
    Output(M0C).unit("g C/m").help("Carbon metabolic pool in step 0");
    Output(M1C).unit("g C/m").help("Carbon metabolic pool in step 1");
    Output(M2C).unit("g C/m").help("Carbon metabolic pool in step 2");
    Output(M3C).unit("g C/m").help("Carbon metabolic pool in step 3");
    Output(M4C).unit("g C/m").help("Carbon metabolic pool in step 4");
    Output(M0N).unit("g N/m").help("Nitrogen metabolic pool in step 0");
    Output(M1N).unit("g N/m").help("Nitrogen metabolic pool in step 0");
    Output(M2N).unit("g N/m").help("Nitrogen metabolic pool in step 0");
    Output(M3N).unit("g N/m").help("Nitrogen metabolic pool in step 0");
    Output(structuralLossC).unit("g C/m").help("Structural carbon lost to respiration");
    Output(sdGrowthC).unit("[0;1]").help("Supply/demand ratio of carbon for growth");
    Output(sdGrowthN).unit("[0;1]").help("Supply/demand ratio of nitrogen for growth");
    Output(sdReservesC).unit("[0;1]").help("Supply/demand ratio of carbon for reserves");
    Output(sdReservesN).unit("[0;1]").help("Supply/demand ratio of nitrogen for reserves");
    Output(structuralLossRate).unit("[0;1]").help("Less than 1 if structuralLossC > 0");
    Output(structuralBiomassGain).unit("g dw/m").help("Growth in structural biomass");
    Output(reservesIter).help("No. of iterations to compute reserve supplies");
}

void Allocation::reset() {
    update();
}

void Allocation::update() {
    // Zero out all outputs, in case we return in step 1
    zeroOut();
    // Step 0. Fill metabolic pools
    M0C = massCRes + supplyC;
    M0N = massNRes + supplyN;

    // Step 1. Maintenance respiration
    supplyRespirationC = demandRespirationC;
    if (demandRespirationC >= M0C) {
        structuralLossC = (M0C - demandRespirationC)/propCStruct;
        structuralLossRate = (massCStruct >0.) ? structuralLossC/massCStruct : 0.;
        return;
    }
    M1C = M0C - supplyRespirationC;
    M1N = M0N;

    // Step 2. Structural growth
    sdGrowthC = min(M1C/demandGrowthC/(1. + costStruct), 1.);
    sdGrowthN = min(M1N/demandGrowthN, 1.);
    supplyGrowthC = min(sdGrowthC, sdGrowthN)*demandGrowthC;
    supplyGrowthN = propNStruct/propCStruct*supplyGrowthC;
    structuralBiomassGain = supplyGrowthC/propCStruct;
    M2C = M1C - supplyGrowthC*(1. + costStruct);
    M2N = M1N - supplyGrowthN;

    // Constraints: [C] and [N] cannot exceed their optima
    reservesIter = 0;
    while (++reservesIter < 100) {
        // Step 3. Allocate N reserves
        // Return original reserves
        M3N = M2N - massNRes;
        // Subtract C costs for additional N reserves
        if (M3N > 0.) {
            M3C = M2C - costNRes*M3N;
            // Reduce N allocation when short of C
            if (M3C < 0.) {
                M3N = M2C/costNRes;
                M3C = 0.;
            }
        }
        // No C cost
        else {
            M3C = M2C;
        }
        // Supply cannot exceed demand
        supplyReservesN = min(M3N, demandReservesN);

        // Step 4. Allocate C reserves
        // Return original reserves
        M4C = M3C - massCRes;
        // Subtract C costs for additional C reserves
        supplyReservesC = (M4C > 0.) ? M4C/(1. + costCRes) : M4C;
        // Supply cannot exceed demand
        supplyReservesC = min(supplyReservesC, demandReservesC);

        // Check constraints on [C] and [N]
        const double step = 0.95;
        double C = concC(supplyReservesC, supplyReservesN),
               N = concN(supplyReservesC, supplyReservesN);
        if (C > optCConc)
            M2C = step*M2C;
        else if (N > optNConc)
            M2N = step*M2N;
        else
          break;
    }
    sdReservesC = (demandReservesC > 0.) ? supplyReservesC/demandReservesC : 0.;
    sdReservesN = (demandReservesN > 0.) ? supplyReservesN/demandReservesN : 0.;
}

void Allocation::zeroOut() {
    supplyRespirationC =
    supplyGrowthC =
    supplyGrowthN =
    supplyReservesC =
    supplyReservesN =
    M0C = M1C = M2C = M3C = M4C =
    M0N = M1N = M2N = M3N =
    structuralLossC =
    structuralBiomassGain =
    sdGrowthC = sdGrowthN =
    structuralLossRate = 0.;
}

double Allocation::biomass(double supplyReservesCGuess, double supplyReservesNGuess) const {
    return (massCStruct + supplyGrowthC)/propCStruct + (massCRes + supplyReservesCGuess)/propCRes + (massNRes + supplyReservesNGuess)/propNRes;
}

double Allocation::concC(double supplyReservesCGuess, double supplyReservesNGuess) const {
    double massC = (massCStruct + supplyGrowthC) + (massCRes + supplyReservesCGuess);
    return massC/biomass(supplyReservesCGuess, supplyReservesNGuess);
}

double Allocation::concN(double supplyReservesCGuess, double supplyReservesNGuess) const {
    double massN = (massNStruct + supplyGrowthN) + (massNRes + supplyReservesNGuess);
    return massN/biomass(supplyReservesCGuess, supplyReservesNGuess);
}

}
