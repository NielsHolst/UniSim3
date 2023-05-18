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
    Input(temperatureCoef).equals(14.).help("Proportionality of air flux with temperature diffence").unit("/h/K");
    Input(crackVentilation).imports("controllers/crackVentilation[value]", CA).help("Minimum air flux through vents").unit("/h");
    Input(windSpeed).imports("outdoors[windSpeed]", CA);
    Input(leakage).imports("construction/leakage[value]", CA);
    Input(outdoorsTemperature).imports("outdoors[temperature]", CA);
    Input(indoorsTemperature).imports("indoors[temperature]", CA);
    Output(value).help("Ventilation air flux, including leakage").unit("/h");
    Output(relative).unit("[0;1]").help("Ventilation relative to maximum possible");
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
    _minValue = sqrt(p2(crackVentilation) + p2(leakage)),
    _maxValue = sqrt(p2(maxFluxWind) + p2(maxFluxTemp) + p2(leakage));
    updateOutput();
}

void ActuatorVentilation::increase(double delta) {
    if (le(desiredValue,_minValue))
        desiredValue = _minValue + delta;
    else
        desiredValue += delta;
    updateOutput();
}

void ActuatorVentilation::updateOutput() {
    value = minmax(_minValue, desiredValue, _maxValue);
    relative = (_maxValue == 0.) ? 0. : value/_maxValue;
    isVentilating = gt(value, _minValue);
}

} //namespace

