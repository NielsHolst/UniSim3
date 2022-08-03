/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ALLOCATION_H
#define ALLOCATION_H
#include <base/box.h>

namespace saccharina {

class Allocation : public base::Box
{
public:
    Allocation(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double
    massCStruct,
    massNStruct,
    massCRes,
    massNRes,
    propCStruct,
    propNStruct,
    propCRes,
    propNRes,
    optCConc,
    optNConc,
    supplyC,
    supplyN,
    demandRespirationC,
    demandGrowthC,
    demandGrowthN,
    demandReservesC,
    demandReservesN,
    demandCostC,
    costStruct,
    costCRes,
    costNRes;
    // Outputs
    double
    supplyRespirationC,
    supplyGrowthC,
    supplyGrowthN,
    supplyReservesC,
    supplyReservesN,
    M0C, M1C, M2C, M3C, M4C,
    M0N, M1N, M2N, M3N,
    structuralLossC,
    structuralLossRate,
    structuralBiomassGain,
    sdGrowthC, sdGrowthN,
    sdReservesC, sdReservesN;
    int reservesIter;
    // Methods
    void zeroOut();
    double biomass(double supplyReservesC, double supplyReservesN) const;
    double concC(double supplyReservesC, double supplyReservesNGuess) const;
    double concN(double supplyReservesCGuess, double supplyReservesNGuess) const;
};

}

#endif
