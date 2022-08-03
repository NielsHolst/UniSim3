/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "demand_carbon_exudation.h"

using namespace base;

namespace saccharina {

PUBLISH(DemandCarbonExudation)

DemandCarbonExudation::DemandCarbonExudation(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates carbon exudation");
    Input(gamma).help("Exudation parameter (h-1)");
    Input(proportionC).imports("sim/structure[proportionC]");
    Input(structuralMass).imports("sim/structure[mass]");
    Input(demandStructure).imports("../structure[value]");
    Input(timeStepSecs).imports("calendar[timeStepSecs]");
    Input(wetWeight).imports("biomass[wetWeight]");
    Output(value).unit("g C").help("Carbon exudation");
    Output(proportion).unit("[0;1]").help("The proportion exuded");
    Output(exudation).unit("micromol C g-1 hour-1");
}

void DemandCarbonExudation::reset() {
   update();
}

void DemandCarbonExudation::update() {
    double dt = timeStepSecs/3600.;
    proportion = 1. - exp(-gamma*dt);
//    value = proportion*(reservesProportion*structuralMass + demandStructure);
    value = proportion*proportionC*structuralMass;
//    value = proportion*reservesProportion*structuralMass;
    exudation = (((value/12.0107)*1000000)/wetWeight)/dt;
}

}
