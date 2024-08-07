/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "growth_lights.h"

using namespace base;
using namespace vector_op;

namespace vg {

PUBLISH(GrowthLights)

GrowthLights::GrowthLights(QString name, Box *parent)
    : LayerAsOutput(name, parent)
{
    help("sums power use and radiation from growth light");
    Input(parEmissionBottomLights).imports("./*[parEmissionBottom]");
    Input(swEmissionBottomLights).imports("./*[swEmissionBottom]");
    Input(lwEmissionBottomLights).imports("./*[lwEmissionBottom]");
    Input(lwEmissionTopLights)   .imports("./*[lwEmissionTop]");
    Input(convectionBottomLights).imports("./*[convectionBottom]");
    Input(convectionTopLights)   .imports("./*[convectionTop]");
    Input(powerUsageLights).imports("./*[powerUsage]");
    Output(parEmissionBottom).help("PAR emission downwards").unit("micromole PAR/m2/s");
    Output(swEmissionBottom).help("Short-wave emission downwards").unit("W/m2");
    Output(lwEmissionBottom).help("Long-wave emission downwards").unit("W/m2");
    Output(lwEmissionTop)   .help("Long-wave emission upwards").unit("W/m2");
    Output(convectionBottom).help("Convective heat downwards").unit("W/m2");
    Output(convectionTop)   .help("Convective heat upwards").unit("W/m2");
    Output(powerUsage).help("Current power usage").unit("W/m2");
}

void GrowthLights::reset() {
    makeTransparent();
    update();
}

void GrowthLights::update() {
    parEmissionBottom = sum(parEmissionBottomLights);
    swEmissionBottom  = sum(swEmissionBottomLights);
    lwEmissionBottom  = sum(lwEmissionBottomLights);
    lwEmissionTop     = sum(lwEmissionTopLights);
    convectionBottom  = sum(convectionBottomLights);
    convectionTop     = sum(convectionTopLights);
    powerUsage        = sum(powerUsageLights);
}

} //namespace

