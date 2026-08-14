/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "demand_nitrogen_reserves.h"

using namespace base;

namespace saccharina {

PUBLISH(DemandNitrogenReserves)

DemandNitrogenReserves::DemandNitrogenReserves(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates demand for nitrogen reserves");
    Input(maxReservesProportionN).imports("reserves/nitrogen[maxProportion]");
    Input(reservesProportionN).imports("reserves/nitrogen[proportion]");
    Input(structuralMass).imports("structure[mass]");
    Input(structProportionC).imports("sim/structure[proportionC]");
    Input(demandStructureC).imports("demand/carbon/structure[value]");
    Output(value).unit("g N").help("Demand for nitrogen reserves ");
}

void DemandNitrogenReserves::reset() {
   update();
}

void DemandNitrogenReserves::update() {
    value = (maxReservesProportionN - reservesProportionN)*structuralMass +
             maxReservesProportionN*demandStructureC/structProportionC;
}

}
