/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "actuator_heat_pump.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(ActuatorHeatPump)

ActuatorHeatPump::ActuatorHeatPump(QString name, Box *parent)
    : Box(name, parent)
{
    help("models a heat pump");
    Input(groundArea).imports("construction/geometry[groundArea]",CA);
    Input(indoorsTemperature).imports("indoors/temperature[value]",CA);
    Input(indoorsRh).imports("indoors/humidity[rh]",CA);
    Input(state).imports("setpoints/elementary/heatPumpsOn[value]",CA).help("Running state relative to full effect").unit("[0;1]");
    Input(number).equals(1).help("Number of heat pumps").unit("0,1,2,...");
    Input(maxCoolingPower).equals(78.4).help("Maximum cooling power").unit("kW");
    Input(coolingEfficiency).equals(0.69).help("Effective proportion of cooling power").unit("[0;1]");
    Input(maxFlowRate).equals(2.5).help("Maximum air flow rate").unit("m3/s");
    Input(maxPowerUserParasitic).equals(2.2).help("Maximum basic power use").unit("kW");
    Input(coolingTemperature).equals(10.).help("Temperature of the cooling aggregate").unit("oC");
    Output(condensationRate).help("Rate of water condensed in the unit").unit("kg/m2/s");
    Output(powerUseCooling).help("Power used for cooling").unit("kW/m2");
    Output(powerUserParasitic).help("Power lost").unit("kW/m2");
    Output(powerUse).help("Power used (total)").unit("kW/m2");
    Output(heat).help("Heat generated").unit("kW/m2");
}

void ActuatorHeatPump::reset() {
    update();
}

void ActuatorHeatPump::update() {
    // Compute for one unit
    double
        coolingLoad = maxCoolingPower/coolingEfficiency*state, // kW
        beforeAh = ahFromRh(indoorsTemperature, indoorsRh), // kg/m3
        afterAh = ahFromRh(coolingTemperature, 100.); // kg/m3
    condensationRate = std::max(beforeAh - afterAh, 0.)*maxFlowRate*state;   // kg/s
    double
        latentLoad = evaporationHeat(indoorsTemperature)*condensationRate*1e-3; // J/kg * kg/s * kW/(1000*W) = kW
    powerUseCooling = coolingLoad;
    powerUserParasitic = maxPowerUserParasitic*state;
    powerUse =  powerUseCooling + powerUserParasitic;
    heat = coolingLoad + latentLoad;
    // Correct for number of units and area
    double k = number/groundArea;
    condensationRate *= k;
    powerUseCooling *= k;
    powerUserParasitic *= k;
    powerUse *= k;
    heat *= k;
}

} //namespace

