/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "heat_transfer_sky.h"

using namespace base;
using namespace::phys_math;

namespace vg {

PUBLISH(HeatTransferSky)

HeatTransferSky::HeatTransferSky(QString name, Box *parent)
    : HeatTransferLayerBase(name, parent)
{
    help("holds the radiative parameters of the sky layer");
    Input(irradiation).imports("outdoors[radiation]",CA);
    Input(propPar).imports("outdoors[propPar]",CA);
    Input(propUv).imports("outdoors[propUv]",CA);
    port("swTransmissivityTop")->equals(0.);
    port("lwTransmissivityTop")->equals(0.);
    port("swTransmissivityBottom")->equals(0.);
    port("lwTransmissivityBottom")->equals(0.);
    port("area")->imports("construction/geometry[coverArea]",CA);
    port("temperature")->imports("outdoors[skyTemperature]",CA);
    port("temperatureTop")->imports("outdoors[skyTemperature]",CA);
    port("temperatureBottom")->imports("outdoors[skyTemperature]",CA);
}

void HeatTransferSky::reset() {
    updateAbsorptivities();
    update();
}

void HeatTransferSky::update() {
    // W/m2 ground
    swFluxDown = irradiation;
////     Anticipate that all UV light will be reflected by the cover (glass); this will increase the proportion of PAR inside
////     parFluxDown = 4.57*swFluxDown*propPar/(1. - propUv);
    // That seemed wrong; this should be correct:
    parFluxDown = 4.57*swFluxDown*propPar;

    // Radiation from sky is only downwards
    updateLwEmission();
    lwFluxUp = 0.;
}

} //namespace

