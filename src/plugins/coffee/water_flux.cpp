/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <algorithm>
#include <base/publish.h>
#include "water_flux.h"

using namespace base;
using namespace std;

namespace coffee {

PUBLISH(WaterFlux)

WaterFlux::WaterFlux(QString name, QObject *parent)
    : Box(name, parent) {
    help("models water flux");
    Input(transpirationCoef).equals(7.1).unit("mm/d").help("Transpiration coefficient");
    Input(rootDepth).imports("coffee[rootDepth]");
    Input(potEvaporation).unit("mm/d").help("Potential evaporation");
    Input(potTranspiration).unit("mm/d").help("Potential transpiration");
    Input(concWaterDry).imports("soil/thresholds[dry]");
    Input(concWaterWilt).imports("soil/thresholds[wilting]");
    Input(concWaterFieldCap).imports("soil/thresholds[fieldCap]");
    Input(concWaterSaturated).imports("soil/thresholds[saturated]");
    Input(concWaterMax).imports("soil/thresholds[concWaterMax]");
    Input(availableWater).imports("waterBudget[availableWater]");
    Input(timeStep).imports("calendar[timeStepDays]");
    Output(evaporation).unit("mm/d").help("Evaporation rate");
    Output(transpiration).unit("mm/d").help("Transpiration rate");
    Output(transpirationRatio).unit("[0;1]").help("Ratio of realised to potential transpiration");
}

void WaterFlux::update() {
    double
        propWater = 0.001*availableWater/rootDepth,  // (m3/m3)
        evaporationIndex = clamp((propWater - concWaterWilt)/(concWaterFieldCap - concWaterWilt), 0., 1.), //(-)
        evaporationDemand = potEvaporation*evaporationIndex,       // (mm/d)
        concWater = concWaterWilt +
               max(0.01, potTranspiration/(potTranspiration+transpirationCoef)*(concWaterFieldCap - concWaterWilt)),  // (m3/m3)
        transpirationIndex = (propWater > concWater) ?
              clamp((concWaterMax - propWater) / (concWaterMax - concWaterSaturated), 0., 1.) :
              clamp((propWater - concWaterWilt)      / (concWater - concWaterWilt), 0., 1.),
        transpirationDemand = potTranspiration*transpirationIndex, // (mm/d)
        waterAvailibilityDry = 1000.*concWaterDry*rootDepth, // (mm)
        evapoTranspiration = evaporationDemand + transpirationDemand,
        propAvailable = (evapoTranspiration > 0) ?
                 min((availableWater - waterAvailibilityDry)/timeStep/evapoTranspiration, 1.) : 0.;

    evaporation   = evaporationDemand  *propAvailable;
    transpiration = transpirationDemand*propAvailable;
    transpirationRatio = (potTranspiration > 0.) ? transpiration/potTranspiration : 1.;

}

} //namespace

