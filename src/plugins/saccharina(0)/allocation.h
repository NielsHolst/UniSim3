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
    Allocation(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double
        structuralProportionC, structuralProportionN, structuralMass,
        curReservesProportionC, curReservesProportionN,
        reservesMaxProportionC, reservesMaxProportionN,
        supplyCarbonTotal, supplyNitrogenTotal,
        demandCarbonRespiration, demandCarbonExudation,
        demandCarbonStructure, demandNitrogenStructure,
        demandCarbonErosion, demandNitrogenErosion;
    // Outputs
    double
        MC0, MC1, MC2, MC3,
        MN0, MN1,
        supplyCarbonStructure, supplyCarbonReserves,
        supplyMassStructure, structuralMassLoss,
        reservesProportionC, reservesProportionN,
        sdCarbon, sdNitrogen;

};

}

#endif
