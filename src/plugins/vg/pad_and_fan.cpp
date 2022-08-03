/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "pad_and_fan.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(PadAndFan)

PadAndFan::PadAndFan(QString name, Box *parent)
    : Box(name, parent)
{
    help("models a pad-and-fan cooling system");
    Input(groundArea).imports("construction/geometry[groundArea]",CA);
    Input(outdoorsTemperature).imports("outdoors/temperature[value]",CA);
    Input(outdoorsRh).imports("outdoors[rh]",CA);
    Input(efficiency).equals(0.8).help("Efficiency at decreasing temperature to wet-bulb temperature").unit("[0;1]");
    Input(maxMassFlowRate).equals(61.53).help("Maximum air mass flow rate").unit("kg/s");
    Input(state).equals(1.).help("Running state relative to full effect").unit("[0;1]");
    Output(coolingPower).help("Rate of water condensed in the unit").unit("kW/m2");
    Output(vapourFlux).help("Power used for cooling").unit("kg/m2/s");
}

void PadAndFan::reset() {
    update();
}

void PadAndFan::update() {
    // Compute for the pad-and-fan unit
    double
        outdoorsTwet = Twet(outdoorsTemperature, outdoorsRh),
        outdoorsSh = shFromRh(outdoorsTemperature, outdoorsRh), // kg/kg
        afterTemperature = outdoorsTemperature - efficiency*(outdoorsTemperature - outdoorsTwet),
        afterRh = rhFromTwet(afterTemperature, outdoorsTwet),
        afterSh = shFromRh(afterTemperature, afterRh), // kg/kg
        flowRate = maxMassFlowRate*state/rhoAir(afterTemperature); // kg/s * / (kg/m3) = m3/s
    coolingPower = flowRate*rhoAir(outdoorsTemperature)*(afterTemperature-outdoorsTemperature)*CpWater/1000.; // m3/s * kg/m3 * K * J/kg/K / 1000 = kW
    vapourFlux =   flowRate*(afterSh - outdoorsSh); // kg/s * kg/kg
    // Correct for number area
    coolingPower /= groundArea;
    vapourFlux /= groundArea;
}

} //namespace

