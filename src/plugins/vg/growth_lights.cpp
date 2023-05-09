/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "growth_lights.h"

using namespace base;
using namespace vector_op;

namespace vg {

PUBLISH(GrowthLights)

GrowthLights::GrowthLights(QString name, Box *parent)
    : Box(name, parent)
{
    help("sums power use and radiation from growth light");
    Input(swEmissionBottomLights).imports("./*[swEmissionBottom]");
    Input(lwEmissionBottomLights).imports("./*[lwEmissionBottom]");
    Input(lwEmissionTopLights).imports("./*[lwEmissionTop]");
    Input(parEmissionBottomLights).imports("./*[parEmissionBottom]");
    Input(powerUsageLights).imports("./*[powerUsage]");
    Output(swEmissionBottom).help("Short-wave emission downwards").unit("W/m2");
    Output(lwEmissionBottom).help("Long-wave emission downwards").unit("W/m2");
    Output(lwEmissionTop).help("Long-wave emission upwards").unit("W/m2");
    Output(parEmissionBottom).help("PAR emission downwards").unit("micromole PAR/m2/s");
    Output(powerUsage).help("Current power usage").unit("W/m2");
}

void GrowthLights::reset() {
    update();
}

void GrowthLights::update() {
    swEmissionBottom  = sum(swEmissionBottomLights);
    lwEmissionBottom  = sum(lwEmissionBottomLights);
    lwEmissionTop     = sum(lwEmissionTopLights);
    parEmissionBottom = sum(parEmissionBottomLights);
    powerUsage        = sum(powerUsageLights);
}

} //namespace

