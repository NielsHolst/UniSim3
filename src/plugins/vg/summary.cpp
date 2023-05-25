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
    Input(heatPipesHeatFlux).imports("actuators/heatPipes[heatFlux]");
    Input(lampsPowerUsage).imports("actuators/growthLights[powerUsage]");
    Input(co2Injection).imports("actuators/co2[value]");
    Input(isSkipping).imports("output/*[isSkipping]");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Output(heatingCost).unit("MJ/m2");
    Output(lampCost).unit("MJ/m2");
    Output(co2Cost).unit("kg CO2/m2");
}

void Summary::update() {
    if (!isSkipping) {
        heatingCost += heatPipesHeatFlux*timeStep*1e-6;
        lampCost    += lampsPowerUsage  *timeStep*1e-6;
        co2Cost     += co2Injection     *timeStep/3600.*1e-3;
    }
}

} //namespace

