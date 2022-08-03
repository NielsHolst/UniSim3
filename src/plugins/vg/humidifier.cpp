/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "humidifier.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(Humidifier)

Humidifier::Humidifier(QString name, Box *parent)
    : Box(name, parent)
{
    help("models a humidifier (fogging)");
    Input(groundArea).imports("construction/geometry[groundArea]",CA);
    Input(greenhouseVolume).imports("construction/geometry[volume]",CA);
    Input(inflowTemperature).imports("outdoors/temperature[value]",CA);
    Input(inflowRh).imports("outdoors[rh]",CA);
    Input(efficiency).equals(0.8).help("Efficiency at decreasing temperature to wet-bulb temperature").unit("[0;1]");
    Input(maxFoggingRate).equals(2).help("Maximum humidifying rate").unit("kg/s");
    Input(state).equals(1.).help("Running state relative to full effect").unit("[0;1]");
    Output(coolingPower).help("Power used for cooling").unit("kW/m2");
    Output(vapourFlux).help("Rate of water humidification").unit("kg/m2/s");
}

void Humidifier::reset() {
    update();
}

void Humidifier::update() {
    // Compute for the humidifier unit
    double
        inflowTwet = Twet(inflowTemperature, inflowRh),
        inflowSh = shFromRh(inflowTemperature, inflowRh), // kg/kg
        outflowTemperature = inflowTemperature - efficiency*(inflowTemperature - inflowTwet),
        outflowRh = rhFromTwet(outflowTemperature, inflowTwet),
        outflowSh = shFromRh(outflowTemperature, outflowRh), // kg/kg
        // kg water / kg air * kg air / m3 air * m3 air / s = kg/s
        demandedFoggingRate = (outflowSh - inflowSh)*rhoAir(inflowTemperature)*greenhouseVolume/timeStep;
    vapourFlux = std::min(demandedFoggingRate, maxFoggingRate*state);
    coolingPower = evaporationHeat(inflowTemperature)*vapourFlux/1000.; // J/kg * kg/s / 1000 = kW
    // Correct for number area
    vapourFlux /= groundArea;
    coolingPower /= groundArea;
}

} //namespace

