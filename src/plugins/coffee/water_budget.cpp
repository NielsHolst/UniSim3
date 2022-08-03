/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "water_budget.h"

using namespace base;
using namespace std;

namespace coffee {

PUBLISH(WaterBudget)

WaterBudget::WaterBudget(QString name, QObject *parent)
    : Box(name, parent) {
    help("models the water budget");
    Input(slope).equals(5.).unit("%").help("Slope of terrain");
    Input(runoffLaiCoef).equals(0.5).unit("mm/m2/m2").help("Lessens runoff with increasing leaf area index");
    Input(concWaterFieldCap).imports("soil/thresholds[fieldCap]");
    Input(rootDepth).imports("coffee[rootDepth]");
    Input(timeStep).imports("calendar[timeStepDays]");
    Input(rainfall).imports("weather[Rain]");
    Input(rainIntercepted).imports("rainInterception[total]");
    Input(laiCoffee).imports("/*/coffee[lai]");
    Input(laiTree).imports("tree[lai]");
    Input(evaporationCoffee).imports("coffee/waterFlux[evaporation]");
    Input(transpirationCoffee).imports("coffee/waterFlux[transpiration]");
    Input(transpirationTree).imports("tree/waterFlux    [transpiration]");
    Output(availableWater).unit("mm").help("Water availibility");
    Output(runoff).unit("mm").help("Amount of runoff water");
    Output(drain).unit("mm").help("Amount of water drained");
}

void WaterBudget::reset() {
    availableWater =
    _availableAtFieldCap = 1000.*concWaterFieldCap*rootDepth;
}

void WaterBudget::update() {
    // Net evaporation (which is from the soil) is calculated by coffee/waterflux;
    // evaporation from tree/waterFlux is ignored; this would be the evaporation if there were no coffee
    double uninterceptedRain = (rainfall - rainIntercepted)*timeStep,
           angle = atan(slope/100.),
           lai = laiCoffee + laiTree,
           evaporation = evaporationCoffee,
           transpiration = transpirationCoffee + transpirationTree;
    runoff = uninterceptedRain*sin(angle)*exp(-runoffLaiCoef*lai);
    drain  = max(0., availableWater - _availableAtFieldCap + uninterceptedRain - runoff - evaporation - transpiration);
    availableWater += uninterceptedRain - evaporation - transpiration - runoff - drain;

}

} //namespace

