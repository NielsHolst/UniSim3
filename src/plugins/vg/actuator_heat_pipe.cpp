/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "actuator_heat_pipe.h"

#include <base/dialog.h>

using namespace base;
using namespace phys_math;
using namespace std;
using namespace TestNum;

namespace vg {
	
PUBLISH(ActuatorHeatPipe)

ActuatorHeatPipe::ActuatorHeatPipe(QString name, Box *parent)
	: Box(name, parent)
{
    help("computes outflow temperature and effect");
    Input(volume).equals(15.).help("Pipe volume").unit("m3");
    Input(flowRate).equals(20.).help("Water flow rate").unit("m3/h");
    Input(k).equals(0.0063).help("Calibration parameter");
    Input(b).equals(1.25).help("Calibration parameter(!=1)");
    Input(propLw).equals(0.5).help("Proportion of energy emitted as long-wave radiation").unit("[0;1]");
    Input(minTemperature).equals(20.).help("Minimum inflow temperature").unit("oC");
    Input(maxTemperature).equals(80.).help("Maximum inflow temperature").unit("oC");
    Input(desiredTemperature).help("Desired inflow temperature").unit("oC");
    Input(indoorsTemperature).imports("indoors[temperature]",CA).unit("oC");
    Input(groundArea).imports("geometry[groundArea]",CA).unit("m2");

    Output(inflowTemperature).help("Water temperature at entry").unit("oC");
    Output(outflowTemperature).help("Water temperature at exit").unit("oC");
    Output(transitTime).help("Average transit time of water").unit("min");
    Output(temperatureDrop).help("Drop in water temperature from entry to exit").unit("oC");
    Output(energyFlux).help("Energy flux").unit("W/m2");
    Output(lwEmissionTop).help("Long-wave emission upwards").unit("W/m2");
    Output(lwEmissionBottom).help("Long-wave emission downwards").unit("W/m2");
    Output(convectionTop).help("Convective heat flux upwards").unit("W/m2");
    Output(convectionBottom).help("Convective heat flux downwards").unit("W/m2");
    Output(isHeating).help("Is heat pipe inflow above minimum temperature?").unit("bool");
}

void ActuatorHeatPipe::reset() {
    desiredTemperature = minTemperature;
    update();
}

void ActuatorHeatPipe::update() {
    transitTime = volume/flowRate*60.;
    inflowTemperature = minmax(minTemperature, desiredTemperature, maxTemperature);
    double dT = inflowTemperature - indoorsTemperature;
    if (dT > 0.) {
        double x = k*(b-1.)*transitTime + pow(dT, 1-b);
        temperatureDrop = dT - pow(x, 1./(1.-b));
        outflowTemperature = inflowTemperature - temperatureDrop;
        energyFlux = CpWater*temperatureDrop*flowRate/groundArea*1000./3600.;
    }
    else {
        temperatureDrop = energyFlux = 0.;
        outflowTemperature = inflowTemperature;
    }
    lwEmissionTop = lwEmissionBottom = propLw*energyFlux/2.;
    // Convection flux is negative because pipes are loosing energy
    convectionTop = convectionBottom =  -(1. - propLw)*energyFlux/2.;
    isHeating = gt(inflowTemperature, minTemperature);
}

void ActuatorHeatPipe::increase(double delta) {
    desiredTemperature = inflowTemperature + delta;
}

}

