/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "actuator_heat_pipe.h"
#include "heat_pipes.h"

using namespace base;
using namespace vector_op;

namespace vg {

PUBLISH(HeatPipes)

HeatPipes::HeatPipes(QString name, Box *parent)
    : LayerAsOutput(name, parent)
{
    help("sums power use and radiation from growth light");
    Input(lwEmissionTopPipes)    .imports("./*[lwEmissionTop]");
    Input(lwEmissionBottomPipes) .imports("./*[lwEmissionBottom]");
    Input(convectionTopPipes)    .imports("./*[convectionTop]");
    Input(convectionBottomPipes) .imports("./*[convectionBottom]");
    Input(inflowTemperaturePipes).imports("./*[inflowTemperature]");
    Input(isHeatingPipes)        .imports("./*[isHeating]");
    Output(lwEmissionTop)   .help("Long-wave emission upwards").unit("W/m2");
    Output(lwEmissionBottom).help("Long-wave emission downwards").unit("W/m2");
    Output(convectionTop)   .help("Convective heat flux upwards").unit("W/m2");
    Output(convectionBottom).help("Convective heat flux downwards").unit("W/m2");
    Output(heatFlux).help("Total heat flux from pipes").unit("W/m2");
    Output(inflowTemperatureAvg).help("Average of inflow temperatures").unit("oC");
    Output(isHeating).help("Is any heat pipe above its minimum temperature?");
}

void HeatPipes::reset() {
    _heatPipes = findMany<ActuatorHeatPipe*>("./*");
    makeTransparent();
    update();
}

void HeatPipes::update() {
    lwEmissionTop        = sum(lwEmissionTopPipes);
    lwEmissionBottom     = sum(lwEmissionBottomPipes);
    convectionTop        = sum(convectionTopPipes);
    convectionBottom     = sum(convectionBottomPipes);
    inflowTemperatureAvg = average(inflowTemperaturePipes);
    isHeating            = any(isHeatingPipes);
    heatFlux = lwEmissionTop + lwEmissionBottom - convectionTop - convectionBottom;
}

} //namespace

