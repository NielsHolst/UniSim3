/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "actuator_heat_pipe.h"

using namespace base;
using namespace vector_op;

namespace vg {

PUBLISH(ActuatorHeatPipe)

ActuatorHeatPipe::ActuatorHeatPipe(QString name, Box *parent)
	: Box(name, parent)
{
    help("computes energy flux from heat pipes");
    Input(temperature).unit("oC").help("Current temperature of heat pipes");
    Input(propConvection).equals(0.50).unit("[0;1]").help("Proportion of pipe energy transferred by convection");
    Input(minTemperatures).imports("./pipes/*[minTemperature]").help("Minimum temperature of each pipe");
    Input(maxTemperatures).imports("./pipes/*[maxTemperature]").help("Maximum temperature of each pipe");
    Input(energyFluxes).imports("./pipes/*[energyFlux]").help("Energy flux from each pipe");
    Output(minTemperature).help("Minimum temperature for any pipe").unit("oC");
    Output(maxTemperature).help("Maximum temperature for any pipe").unit("oC");
    Output(energyFluxConvection).help("Energy flux by convection").unit("W/m2");
    Output(energyFluxRadiation).help("Energy flux by convection").unit("W/m2");
    Output(energyFluxTotal).help("Energy flux total").unit("W/m2");
}

void ActuatorHeatPipe::reset() {
    minTemperature = min(minTemperatures);
    maxTemperature = max(maxTemperatures);
    temperature = minTemperature;
    update();
}

void ActuatorHeatPipe::update() {
    energyFluxTotal = sum(energyFluxes);
    energyFluxConvection = propConvection*energyFluxTotal;
    energyFluxRadiation = energyFluxTotal - energyFluxConvection;
}

double ActuatorHeatPipe::getTemperature() const {
    return temperature;
}

void ActuatorHeatPipe::setTemperature(double value) {
    temperature = value;
}

} //namespace

