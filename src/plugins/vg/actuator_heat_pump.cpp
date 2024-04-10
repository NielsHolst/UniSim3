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
    Input(productName).help("Name of product holding heat pump parameters");
    Input(number).equals(1).help("Number of heat pumps");
    Input(groundArea).imports("gh/geometry[groundArea]",CA);
    Input(indoorsTemperature).imports("indoors[temperature]",CA);
    Input(indoorsRh).imports("indoors[rh]",CA);
    Input(state).help("Running state relative to full effect").unit("[0;1]");
//    Input(destination).equals("outdoors").help("Destination of generated heat: 'outdoors' or 'buffer'");

    Output(maxCoolingLoad).help("Maximum cooling power").unit("kW");
    Output(condensationRate).help("Rate of water condensed in the unit").unit("kg/m2/s");
    Output(powerUseCooling).help("Power used for cooling").unit("W/m2");
    Output(powerUserParasitic).help("Power lost").unit("W/m2");
    Output(powerUse).help("Power used (total)").unit("W/m2");
    Output(cooling).help("Cooling effect >= 0. An equal amount of heat is available for any `HeatBuffer`").unit("W/m2");
//    Output(energyToBuffer).help("Cooling energy >= 0, routed to heat buffer").unit("W");
}

#define UPDATE_INPUT(x) x = product->port(#x)->value<double>()

void ActuatorHeatPump::reset() {
    // Copy product parameters
    Box *product = findOne<Box*>("../products/" + productName);
    UPDATE_INPUT(maxCoolingLoad);
    UPDATE_INPUT(coolingEfficiency);
    UPDATE_INPUT(maxFlowRate);
    UPDATE_INPUT(parasiticLoad);
    UPDATE_INPUT(coolingTemperature);

    // Determine destination
//    if (destination.toLower() == "outdoors")
//        useBuffer_ = false;
//    else if (destination.toLower() == "buffer")
//        useBuffer_ = false;
//    else
//        ThrowException("The destination must be 'outdoors' or 'buffer").value(destination).context(this);
    update();
}

void ActuatorHeatPump::update() {
    // Compute for one unit
    const double
        coolingLoad = maxCoolingLoad/coolingEfficiency*state, // kW
        beforeAh = ahFromRh(indoorsTemperature, indoorsRh), // kg/m3
        afterAh = ahFromRh(coolingTemperature, 100.); // kg/m3
    condensationRate = std::max(beforeAh - afterAh, 0.)*maxFlowRate*state;   // kg/s
    powerUseCooling = coolingLoad;
    powerUserParasitic = parasiticLoad*state;
    powerUse = powerUseCooling + powerUserParasitic;
    cooling = coolingLoad + LHe*condensationRate*1e-3; // J/kg * kg/s * kW/(1000*W) = kW

    // Correct for number of units
    condensationRate *= number;
    // Also convert kW -> W and per area
    const double k = 1000.*number/groundArea;
    powerUseCooling *= k;
    powerUserParasitic *= k;
    powerUse *= k;
    cooling *= k;
    // Convert condensation to per area
    condensationRate /= groundArea; // kg/m2/s
    // Route cooling enerygy to destination
//    energyToBuffer = useBuffer_ ? cooling*groundArea : 0.;
}

} //namespace

