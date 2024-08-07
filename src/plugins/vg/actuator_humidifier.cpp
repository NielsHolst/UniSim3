/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
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
    help("models a humidifier");
    Input(productName).help("Name of product holding humidifier parameters");
    Input(number).unit("int").help("Number of humidifiers installed");
    Input(state).equals(1.).help("Running state relative to full effect").unit("[0;1]");
    Input(groundArea).imports("gh/geometry[groundArea]",CA);
    Input(greenhouseVolume).imports("gh/geometry[volume]",CA);
    Input(inflowTemperature).imports("outdoors[temperature]",CA);
    Input(inflowRh).imports("outdoors[rh]",CA);
    Input(timeStep).imports("calendar[timeStepSecs]",CA);
    Output(powerUse).help("Power used for cooling").unit("W/m2");
    Output(vapourFlux).help("Rate of water humidification").unit("kg/m2/s");
}

#define UPDATE_INPUT(x) x = product->port(#x)->value<double>()

void ActuatorHumidifier::reset() {
    if (productName.toLower() == "none") {
        number = 0;
        return;
    }
    Box *product = findOne<Box*>("../products/" + productName);
    UPDATE_INPUT(maxRate)*number;
    UPDATE_INPUT(maxPowerUse)*number;
    UPDATE_INPUT(parasiticLoad)*number;
    update();
}

void ActuatorHumidifier::update() {
    powerUse = vapourFlux = 0.;
//    if (state == 0.) {
//        powerUse = vapourFlux = 0.;
//        return;
//    }
//    double
//        inflowTwet = Twet(inflowTemperature, inflowRh),
//        inflowSh = shFromRh(inflowTemperature, inflowRh), // kg/kg
//        outflowTemperature = inflowTemperature - efficiency*(inflowTemperature - inflowTwet),
//        outflowRh = rhFromTwet(outflowTemperature, inflowTwet),
//        outflowSh = shFromRh(outflowTemperature, outflowRh), // kg/kg
//        demandedVapourFlux = (outflowSh - inflowSh)*rhoAir(inflowTemperature)*greenhouseVolume/timeStep;
//            // kg/s = kg water / kg air * kg air / m3 air * m3 air / s
//    vapourFlux = std::min(demandedVapourFlux, maxRate*state);
//    powerUse = evaporationHeat(inflowTemperature)*vapourFlux + 1000.*parasiticLoad; // W = J/kg * kg/s
//    // Correct for ground area
//    vapourFlux /= groundArea;
//    powerUse /= groundArea;
}

} //namespace

