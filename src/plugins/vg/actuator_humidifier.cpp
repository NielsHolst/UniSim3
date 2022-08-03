/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "actuator_humidifier.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(ActuatorHumidifier)

ActuatorHumidifier::ActuatorHumidifier(QString name, Box *parent)
    : Box(name, parent)
{
    help("models a humidifier (fogging)");
    Input(groundArea).imports("construction/geometry[groundArea]",CA);
    Input(greenhouseVolume).imports("construction/geometry[volume]",CA);
    Input(inflowTemperature).imports("outdoors[temperature]",CA);
    Input(inflowRh).imports("outdoors[rh]",CA);
    Input(state).imports("setpoints/elementary/humidifiersOn[value]",CA).help("Running state relative to full effect").unit("[0;1]");
    Input(efficiency).equals(0.8).help("Efficiency at decreasing temperature to wet-bulb temperature").unit("[0;1]");
    Input(maxHumidification).equals(2).help("Maximum humidification rate").unit("kg/s");
    Input(maxPowerUserParasitic).equals(2.2).help("Maximum basic power use").unit("kW");
    Output(coolingPower).help("Rate of cooling").unit("kW/m2");
    Output(vapourFlux).help("Rate of water humidification").unit("kg/m2/s");
}

void ActuatorHumidifier::reset() {
    update();
}

void ActuatorHumidifier::update() {
    // Compute for the humidifier unit
    double
        beforeTwet = Twet(inflowTemperature, inflowRh),
        beforeSh = shFromRh(inflowTemperature, inflowRh), // kg/kg
        afterTemperature = inflowTemperature - efficiency*(inflowTemperature - beforeTwet),
        afterRh = rhFromTwet(afterTemperature, beforeTwet),
        afterSh = shFromRh(afterTemperature, afterRh), // kg/kg
        // kg water / kg air * kg air / m3 air * m3 air / s = kg/s
        demandedFoggingRate = (afterSh - beforeSh)*rhoAir(inflowTemperature)*greenhouseVolume/timeStep;
    vapourFlux = std::min(demandedFoggingRate, maxHumidification*state);
    coolingPower = evaporationHeat(inflowTemperature)*vapourFlux/1000.; // J/kg * kg/s / 1000 = kW
    // Correct for number area
    vapourFlux /= groundArea;
    coolingPower /= groundArea;
}

} //namespace

