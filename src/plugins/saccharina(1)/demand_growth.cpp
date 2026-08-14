/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "demand_growth.h"

using namespace base;

namespace saccharina {

PUBLISH(DemandGrowth)

DemandGrowth::DemandGrowth(QString name, Box *parent)
    : Box(name, parent)
{
    help("calculates demand for structural growth");
    Input(maxGrowthRate).equals(0.045).unit("d-1").help("Maximum relative growth rate");
    Input(massCStruct).imports("plant[massCStruct]");
    Input(propCStruct).imports("plant[propCStruct]");
    Input(propNStruct).imports("plant[propNStruct]");
    Input(fTemp).unit("[0;1]").equals(1.).help("Temperature scaling of growth rate");
    Input(fSalinity).unit("[0;1]").equals(1.).help("Salinity scaling of growth rate");
    Input(timeStep).imports("calendar[timeStepDays]");
    Output(carbon).unit("g C/m").help("Carbon demand for structural growth");
    Output(nitrogen).unit("g N/m").help("Nitrogen demand for structural growth");
}

void DemandGrowth::reset() {
   update();
}

void DemandGrowth::update() {
    carbon = maxGrowthRate*fTemp*fSalinity*massCStruct*timeStep;
    nitrogen = propNStruct/propCStruct*carbon;
}

}
