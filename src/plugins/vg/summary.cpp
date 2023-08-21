/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "summary.h"

using namespace base;

namespace vg {
	
PUBLISH(Summary)

Summary::Summary(QString name, Box *parent)
	: Box(name, parent)
{
    help("summarises greenhouse budget");
    Input(heatPipesHeatFlux).imports("actuators/heatPipes[heatFlux]").unit("W/m2");
    Input(lampsPowerUsage).imports("actuators/growthLights[powerUsage]").unit("W/m2");
    Input(co2Injection).imports("actuators/co2[value]").unit("g/m2/h");
    Input(isSkipping).imports("output/*[isSkipping]");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Output(heatingCost).unit("MWh/m2/y");
    Output(lampCost).unit("MWh/m2/y");
    Output(co2Cost).unit("kg CO2/m2/y");
}

void Summary::reset() {
    _heatingSum = _lampSum = _co2Sum = 0.;
    _n = 0;
}

void Summary::update() {
    if (!isSkipping) {
        // Update sums
        ++_n;
        _heatingSum += heatPipesHeatFlux;
        _lampSum    += lampsPowerUsage;
        _co2Sum     += co2Injection;
        // Update costs
        heatingCost = _heatingSum/_n*24*365*1e-6;
        lampCost    = _lampSum/_n   *24*365*1e-6;
        co2Cost     = _co2Sum/_n    *24*365*1e-3;
    }
}

} //namespace

