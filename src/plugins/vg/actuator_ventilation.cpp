/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
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
    Input(ventAreaRatio).equals(0.40).unit("int").help("Total vent area in proportion to groundarea");
    Input(windCoef).equals(50.).help("Proportionality of air flux with windspeed").unit("/h/(m/s)");
    Input(temperatureCoef).equals(14.).help("Proportionality of air flux with temperature difference").unit("/h/K");
    Input(opening).unit("[0;1]").help("Ventilation relative to `maxVentFlux`");
    Input(windSpeed).imports("outdoors[windSpeed]", CA);
    Input(leakage).imports("gh/construction/leakage[value]", CA);
    Input(crackOpening).imports("gh/controllers/ventilation/crack[value]", CA);
    Input(outdoorsTemperature).imports("outdoors[temperature]", CA);
    Input(indoorsTemperature).imports("gh/budget/indoors[temperature]", CA);
    Input(outdoorsRh).imports("outdoors[rh]", CA);
    Input(indoorsRh).imports("gh/budget/indoors[rh]", CA);
    Input(timeStep).imports("budget[subTimeStep]");

    Output(ventFlux).help("Ventilation air flux").unit("/h");
    Output(maxVentFlux).help("Maximum possible air flux, including leakage").unit("/h");
    Output(value).help("Total air flux through `ventFlux` and `leakage`").unit("/h");
    Output(isVentilating).help("Is ventilation active, i.e. `opening>crackOpening` ?");
}

void ActuatorVentilation::reset() {
    opening = crackOpening;
    update();
}

void ActuatorVentilation::update() {
    // Compute the max. possible ventilation flux
    double maxFluxWind = ventAreaRatio*windCoef*windSpeed,
           maxFluxTemp = ventAreaRatio*temperatureCoef*std::max(indoorsTemperature - outdoorsTemperature, 0.);
    // Use vector addition
    maxVentFlux = sqrt(p2(maxFluxWind) + p2(maxFluxTemp));
    updateOutput();
}

inline double flux(double opening) {
    return opening;
//    return 1. - pow(1. - opening, 4.);
}

void ActuatorVentilation::updateOutput() {
    ventFlux = flux(std::max(opening, crackOpening))*maxVentFlux;
    value = sqrt(p2(ventFlux) + p2(leakage));
    isVentilating = gt(opening, crackOpening);
}

} //namespace

