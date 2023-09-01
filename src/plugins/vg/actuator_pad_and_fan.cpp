/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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
    Input(groundArea).imports("gh/geometry[groundArea]",CA);
    Input(outdoorsTemperature).imports("outdoors[temperature]",CA);
    Input(outdoorsRh).imports("outdoors[rh]",CA);
    Input(state).imports("setpoints/elementary/padAndFansOn[value]",CA).help("Running state relative to full effect").unit("[0;1]");
    Input(efficiency).equals(0.8).help("Efficiency at decreasing temperature to wet-bulb temperature").unit("[0;1]");
    Input(maxFlowRate).equals(50.).help("Maximum air flow rate").unit("m3/s");
    Input(maxPowerUserParasitic).equals(2.2).help("Maximum basic power use").unit("kW");
    Output(coolingPower).help("Rate of water condensed in the unit").unit("kW/m2");
    Output(vapourFlux).help("Power used for cooling").unit("kg/m2/s");
}

void ActuatorPadAndFan::reset() {
    update();
}

void ActuatorPadAndFan::update() {
    // Compute for the pad-and-fan unit
    double
        beforeTwet = Twet(outdoorsTemperature, outdoorsRh),
        beforeSh = shFromRh(outdoorsTemperature, outdoorsRh), // kg/kg
        afterTemperature = outdoorsTemperature - efficiency*(outdoorsTemperature - beforeTwet),
        afterRh = rhFromTwet(afterTemperature, beforeTwet),
        afterSh = shFromRh(afterTemperature, afterRh), // kg/kg
        flowRate = maxFlowRate*state; // m3/s
    coolingPower = flowRate*rhoAir(outdoorsTemperature)*(afterTemperature-outdoorsTemperature)*CpWater/1000.; // m3/s * kg/m3 * K * J/kg/K / 1000 = kW
    vapourFlux =   flowRate*(afterSh - beforeSh); // kg/s * kg/kg
    // Correct for number area
    coolingPower /= groundArea;
    vapourFlux /= groundArea;
}

} //namespace

