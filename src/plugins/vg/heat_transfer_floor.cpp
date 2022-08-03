/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "heat_transfer_floor.h"

using namespace base;

namespace vg {

PUBLISH(HeatTransferFloor)

HeatTransferFloor::HeatTransferFloor(QString name, Box *parent)
    : HeatTransferLayerBase(name, parent)
{
    help("holds the radiative parameters of the floor");
    port("swTransmissivityTop")->equals(0.);
    port("swTransmissivityBottom")->equals(0.);
    port("swReflectivityTop")->imports("construction/floor[reflectivity]");
    port("swReflectivityBottom")->imports("construction/floor[reflectivity]");
    port("lwTransmissivityTop")->equals(0.);
    port("lwTransmissivityBottom")->equals(0.);
    port("lwReflectivityTop")->imports("construction/floor[reflectivity]");
    port("lwReflectivityBottom")->imports("construction/floor[reflectivity]");
    port("Utop")->imports("construction/floor[Utop]");
    port("Ubottom")->imports("construction/floor[Ubottom]");
    port("heatCapacity")->imports("construction/floor[heatCapacity]");
    port("area")->imports("construction/geometry[groundArea]",CA);
}

void HeatTransferFloor::reset() {
    updateAbsorptivities();
    temperature = temperatureTop = temperatureBottom= 20.;
}

void HeatTransferFloor::update() {
    updateLwEmission();
    lwFluxDown = 0;
    updateTemperature();
    if (temperature > 100.)
        ThrowException("Unrealistic, high floor temperature").value(temperature).
                hint("Increase floor's U-values").context(this);

}

} //namespace

