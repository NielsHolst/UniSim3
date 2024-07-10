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
    Input(state).imports("controllers/padAndFans[state]");
    Input(sendToBuffer).equals(true).help("Send heat to buffer?");

    Output(maxCoolingLoad).help("From product").unit("kW");
    Output(condensation).help("Rate of water condensed in the unit").unit("kg/m2/s");
    Output(powerUseCooling).help("Power used for cooling").unit("W/m2");
    Output(powerUserParasitic).help("Power lost").unit("W/m2");
    Output(powerUse).help("Power use (total)").unit("W/m2");
    Output(cooling).help("Cooling effect >= 0. An equal amount of heat is available for any `HeatBuffer`").unit("W/m2");
    Output(energyToBuffer).help("Cooling energy >= 0, sent to heat buffer").unit("W/m2");
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
    update();
}

void ActuatorHeatPump::update() {
    // Compute for one unit
    powerUseCooling = maxCoolingLoad/coolingEfficiency*state; // kW
    const double
        beforeAh = ahFromRh(indoorsTemperature, indoorsRh), // kg/m3
        afterAh = ahFromRh(coolingTemperature, 100.); // kg/m3
    condensation = std::max(beforeAh - afterAh, 0.)*maxFlowRate*state;   // kg/s
    powerUserParasitic = parasiticLoad*state;
    powerUse = powerUseCooling + powerUserParasitic;
    cooling = powerUseCooling + LHe*condensation*1e-3; // J/kg * kg/s * kW/(1000*W) = kW
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

