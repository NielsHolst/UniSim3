/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "actuator_pad_and_fan.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(ActuatorPadAndFan)

ActuatorPadAndFan::ActuatorPadAndFan(QString name, Box *parent)
    : Box(name, parent)
{
    help("models a pad-and-fan cooling system");
    Input(productName).help("Name of product holding pad and fan parameters");
    Input(number).equals(1).help("Number of pad and fan units");
    Input(state).help("Running state relative to full effect").unit("[0;1]");
    Input(groundArea).imports("gh/geometry[groundArea]",CA);
    Input(outdoorsTemperature).imports("outdoors[temperature]",CA);
    Input(outdoorsRh).imports("outdoors[rh]",CA);
    Output(vapourFlux).help("Rate of evaporation").unit("kg/m2/s");
    Output(cooling).help("Cooling achieved by evaporation").unit("W/m2");
    Output(powerUse).help("Power use including parasitic").unit("W/m2");
}

#define UPDATE_INPUT(x) x = product->port(#x)->value<double>()

void ActuatorPadAndFan::reset() {
    // Copy product parameters
    Box *product = findOne<Box*>("../products/" + productName);
    UPDATE_INPUT(coolingEfficiency);
    UPDATE_INPUT(powerUseMax);
    UPDATE_INPUT(flowRateMax);
    UPDATE_INPUT(powerUserParasitic);
    update();
}

void ActuatorPadAndFan::update() {
    const double
        wetT = Twet(outdoorsTemperature, outdoorsRh),
        incomingAirT = outdoorsTemperature - coolingEfficiency*(outdoorsTemperature - wetT);
    cooling = number*(outdoorsTemperature - incomingAirT)*CpAirVol*flowRateMax*state/groundArea; // K * J/m3/K * m3/s / m2 = W/m2
    vapourFlux = cooling/LHe; // W/m2 / (J/kg) = kg/m2/s
    powerUse = number*(powerUseMax*state + powerUserParasitic);
}

} //namespace

