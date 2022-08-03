/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "heat_transfer_growth_light.h"

using namespace base;

namespace vg {

PUBLISH(HeatTransferGrowthLight)

HeatTransferGrowthLight::HeatTransferGrowthLight(QString name, Box *parent)
    : HeatTransferLayerBase(name, parent)
{
    help("holds the radiative parameters of the growth lights");
    port("area")->imports("construction/geometry[groundArea]",CA);
    port("swFluxDown")->imports("actuators/growthLights[shortWaveIntensity]",CA);
    port("lwFluxDown")->imports("actuators/growthLights[longWaveIntensity]",CA);
}

void HeatTransferGrowthLight::initialize() {
    swReflectivityBottom = swReflectivityTop =
    lwReflectivityTop = lwReflectivityTop = 0.;
    swTransmissivity = lwTransmissivity = 1.;
    updateAbsorptivities();
}

} //namespace

