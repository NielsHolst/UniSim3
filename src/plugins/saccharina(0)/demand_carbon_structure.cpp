/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "demand_carbon_structure.h"

using namespace base;

namespace saccharina {

PUBLISH(DemandCarbonStructure)

DemandCarbonStructure::DemandCarbonStructure(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates demand for structural growth");
    Input(maxGrowthRate).unit("h-1").help("Maximum relative growth rate");
    Input(proportionC).imports("sim/structure[proportionC]");
    Input(structuralMass).imports("sim/structure[mass]");
    Input(fTemp).equals(1.).help("Temperature scaling of growth rate");
    Input(fSalinity).equals(1.).help("Salinity scaling of growth rate");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Output(value).unit("g C").help("Carbon demand for structural growth");
}

void DemandCarbonStructure::reset() {
   update();
}

void DemandCarbonStructure::update() {
    value = maxGrowthRate*proportionC*structuralMass*fTemp*fSalinity*timeStep/3600.; //proportionC added 19/11/2020
}

}
