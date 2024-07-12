/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
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
    Input(state).imports("controllers/heatPumps/state[value]");
    Input(sendToBuffer).equals(true).help("Send heat to buffer?");
    Output(maxCoolingLoad).help("From product (with converted units )").unit("W/m2");
    Output(condensation).help("Rate of water condensed in the unit").unit("kg/m2/s");
    Output(powerUseCooling).help("Power used for cooling").unit("W/m2");
    Output(powerUserParasitic).help("Power lost").unit("W/m2");
    Output(powerUse).help("Power use (total)").unit("W/m2");
    Output(cooling).help("Cooling effect >= 0. An equal amount of heat is available for any `HeatBuffer`").unit("W/m2");
    Output(energyToBuffer).help("Cooling energy >= 0, sent to heat buffer").unit("W/m2");
}

void ActuatorHeatPump::reset() {
    // Copy product parameters
    Box *product = findOne<Box*>("../products/" + productName);
    p.maxCoolingLoad     = product->port("maxCoolingLoad")->value<double>();
    p.coolingEfficiency  = product->port("coolingEfficiency")->value<double>();
    p.maxFlowRate        = product->port("maxFlowRate")->value<double>();
    p.parasiticLoad      = product->port("parasiticLoad")->value<double>();
    p.coolingTemperature = product->port("coolingTemperature")->value<double>();
    update();
    // Correct max load for number of units and area
    double k = 1000.*number/groundArea;
    maxCoolingLoad = k*p.maxCoolingLoad;
}

void ActuatorHeatPump::update() {
    // Compute for one unit
    // Condensation
    const double
        beforeAh = ahFromRh(indoorsTemperature, indoorsRh), // kg/m3
        afterAh = ahFromRh(p.coolingTemperature, 100.); // kg/m3
    condensation = std::max(beforeAh - afterAh, 0.)*p.maxFlowRate*state;   // kg/s

    // Power use
    powerUseCooling = std::max(p.maxCoolingLoad - p.parasiticLoad, 0.)*state; // kW
    powerUserParasitic = p.parasiticLoad*state;
    powerUse = powerUseCooling + powerUserParasitic;
    cooling = powerUseCooling*p.coolingEfficiency + LHe*condensation*1e-3; // J/kg * kg/s * kW/(1000*W) = kW

    energyToBuffer = sendToBuffer ? cooling : 0.;

    // Correct condensation for number of units and area
    double k = number/groundArea;
    condensation *= k;

    // For energy also convert kW -> W
    k *= 1000.;
    powerUseCooling *= k;
    powerUserParasitic *= k;
    powerUse *= k;
    cooling *= k;
    energyToBuffer *= k;
}

} //namespace

