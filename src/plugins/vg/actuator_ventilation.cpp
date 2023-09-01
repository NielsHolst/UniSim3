/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "actuator_ventilation.h"

using namespace base;
using namespace phys_math;
using namespace TestNum;

namespace vg {

PUBLISH(ActuatorVentilation)

ActuatorVentilation::ActuatorVentilation(QString name, Box *parent)
	: Box(name, parent)
{
    help("sets air flux through vents");
    Input(desiredValue).unit("/h").help("Desired ventilation rate");
    Input(ventAreaRatio).equals(0.40).unit("int").help("Total vent area in proportion to groundarea");
    Input(windCoef).equals(50.).help("Proportionality of air flux with windspeed").unit("/h/(m/s)");
    Input(temperatureCoef).equals(14.).help("Proportionality of air flux with temperature difference").unit("/h/K");
    Input(windSpeed).imports("outdoors[windSpeed]", CA);
    Input(leakage).imports("gh/construction/leakage[value]", CA);
    Input(outdoorsTemperature).imports("outdoors[temperature]", CA);
    Input(indoorsTemperature).imports("gh/budget/indoors[temperature]", CA);
    Input(outdoorsRh).imports("outdoors[rh]", CA);
    Input(indoorsRh).imports("gh/budget/indoors[rh]", CA);
    Input(timeStep).imports("calendar[timeStepSecs]");
    Input(indoorsVolume).imports("gh/geometry[volume]");

    Output(value).help("Ventilation air flux, including leakage").unit("/h");
    Output(minValue).help("Minimum possible air flux, including leakage").unit("/h");
    Output(maxValue).help("Maximum possible air flux, including leakage").unit("/h");
    Output(relative).unit("[0;1]").help("Ventilation relative to maximum possible");
    Output(sensibleHeatFlux).unit("W/m2").help("Sensible heat flux");
    Output(latentHeatFlux).unit("W/m2").help("Latent heat flux");
    Output(isVentilating).unit("bool").help("Are vents more open than crack?");
}

void ActuatorVentilation::reset() {
    desiredValue = leakage;
    update();
}

void ActuatorVentilation::update() {
    // Compute the max. possible ventilation flux
    double maxFluxWind = ventAreaRatio*windCoef*windSpeed,
           maxFluxTemp = ventAreaRatio*temperatureCoef*std::max(indoorsTemperature - outdoorsTemperature, 0.);
    // Use vector addition
    minValue = leakage,
    maxValue = sqrt(p2(maxFluxWind) + p2(maxFluxTemp) + p2(leakage));
    maxValue = std::max(minValue, maxValue);
    updateOutput();
}

void ActuatorVentilation::increase(double delta) {
    desiredValue = value + delta;
    updateOutput();
}

void ActuatorVentilation::decrease(double delta, double relative) {
    double change = std::max(delta, relative*value);
    desiredValue = value - change;
    updateOutput();
}

void ActuatorVentilation::updateOutput() {
    value = minmax(minValue, desiredValue, maxValue);
    relative = (maxValue == 0.) ? 0. : (value-leakage)/(maxValue-leakage);
    isVentilating = gt(value, minValue);
    // Energetics
    double
        deltaT = outdoorsTemperature - indoorsTemperature,
        deltaAh = ahFromRh(outdoorsTemperature, outdoorsRh) -
                  ahFromRh(indoorsTemperature, indoorsRh),
        deltaV = (1. - exp(-value/3600.))*indoorsVolume;
    sensibleHeatFlux = deltaV*deltaT*CpAirVol/timeStep;
    latentHeatFlux   = deltaV*deltaAh*LHe/timeStep;
}

} //namespace

