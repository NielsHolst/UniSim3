/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/test_num.h>
#include "heat_transfer_crop.h"

using namespace base;

namespace vg {

PUBLISH(HeatTransferCrop)

HeatTransferCrop::HeatTransferCrop(QString name, Box *parent)
    : HeatTransferLayerBase(name, parent)
{
    help("holds the radiative parameters of the crop layer");
    Input(lai).imports("/crop[lai]",CA);
    Input(swK).imports("crop[k]",CA);
    Input(swScatteringCoef).equals(0.2);
    Input(lwK).equals(0.80);
    Input(lwScatteringCoef).equals(0.05);
    Input(leafTemperature).imports("crop/temperature[value]",CA);
    port("area")->imports("construction/geometry[groundArea]",CA);
}


void HeatTransferCrop::reset() {
    temperature = 20.;
}

void HeatTransferCrop::update() {
    temperature = temperatureTop = temperatureBottom = leafTemperature;

    Distribution sw = distribute(swK, swScatteringCoef);
    swAbsorptivityBottom   = swAbsorptivityTop   = sw.a;
    swReflectivityBottom   = swReflectivityTop   = sw.r;
    swTransmissivityBottom = swTransmissivityTop = sw.t;

    Distribution lw = distribute(lwK, lwScatteringCoef);
    lwAbsorptivityBottom   = lwAbsorptivityTop   = lw.a;
    lwReflectivityBottom   = lwReflectivityTop   = lw.r;
    lwTransmissivityBottom = lwTransmissivityTop = lw.t;

    updateLwEmission();
}

HeatTransferCrop::Distribution  HeatTransferCrop::distribute(double k, double scatteringCoef) const {
    // Goudriaan (1977), eq. 2.26 with rs==0
    double &sigma(scatteringCoef),
           rh = (1.-sqrt(1.-sigma))/(1.+sqrt(1.-sigma));
    Distribution d;
    d.r = (exp(-k*lai) - exp(k*lai)) / (rh*exp(-k*lai) - exp(k*lai)/rh);
    d.a = (1. - d.r)*(1. - exp(-k*lai));
    d.t = 1. - d.r - d.a;
    return d;

}

} //namespace

