/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "actuator_ventilation.h"

using namespace base;
using namespace phys_math;
using namespace vector_op;

namespace vg {

PUBLISH(ActuatorVentilation)

ActuatorVentilation::ActuatorVentilation(QString name, Box *parent)
	: Box(name, parent)
{
    help("sets air flux through vents");
    Input(minFlux).imports("setpoints/crackVentilation[value]", CA).help("Minimum air flux through vents").unit("/h");
    Input(windCoef).equals(50.).help("Proportionality of air flux with windspeed").unit("/h/(m/s)");
    Input(temperatureCoef).equals(14.).help("Proportionality of air flux with temperature diffence").unit("/h/K");
    Input(windSpeed).imports("outdoors[windSpeed]", CA);
    Input(outdoorsTemperature).imports("outdoors[temperature]", CA);
    Input(indoorsTemperature).imports("indoors/temperature[value]", CA);
    Input(effectiveVentArea).computes("sum(shelter/*/vent[effectiveArea])");
    Input(groundArea).imports("construction/geometry[groundArea]", CA);
    Output(flux).help("Current air flux through vents").unit("/h");
    Output(maxFlux).help("Maximum air flux through vents").unit("/h");
    Output(relative).help("Flux as proportion of max. at fully open vents").unit("[0;1]");
}

void ActuatorVentilation::reset() {
    flux = minFlux;
}

void ActuatorVentilation::update() {
    // Compute the max. possible ventilation flux
    double ventEffect  = effectiveVentArea/groundArea,
           maxFluxWind = ventEffect*windCoef*windSpeed,
           maxFluxTemp = ventEffect*temperatureCoef*std::max(indoorsTemperature-outdoorsTemperature, 0.);
    maxFlux = sqrt(p2(maxFluxWind) + p2(maxFluxTemp));
    // Set flux within bounds
    flux = minmax(minFlux, flux, maxFlux);
    relative = (maxFlux < 0.001) ? 0. : flux/maxFlux;
}

double ActuatorVentilation::getFlux() const {
    return flux;
}

void ActuatorVentilation::setFlux(double newFlux) {
    flux = newFlux;
    update();
}

} //namespace

