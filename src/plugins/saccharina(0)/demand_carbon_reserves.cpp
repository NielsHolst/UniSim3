/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <cmath>
#include <base/publish.h>
#include "demand_carbon_reserves.h"

using namespace base;
using namespace std;

namespace saccharina {

PUBLISH(DemandCarbonReserves)

DemandCarbonReserves::DemandCarbonReserves(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates demand for carbon reserves (new)");
    Input(maxReservesProportionC).imports("reserves/carbon[maxProportion]");
    Input(reservesProportionC).imports("reserves/carbon[proportion]");
    Input(structuralMass).imports("structure[mass]");
    Input(structProportionC).imports("sim/structure[proportionC]");
    Input(demandStructureC).imports("../structure[value]");
//    Input(maxRelativeDemand).equals(10.);
    Output(value).unit("g C").help("Demand for carbon reserves ");
}

void DemandCarbonReserves::reset() {
   update();
}

void DemandCarbonReserves::update() {
    double
        curReservesDemand = (maxReservesProportionC - reservesProportionC)*structuralMass,
        growthReservesDemand = maxReservesProportionC/structProportionC*demandStructureC;
    value = curReservesDemand + growthReservesDemand;
//    double maxValue = maxRelativeDemand*demandStructureC;
//    if (value > maxValue)
//            value = maxValue;
}

}
