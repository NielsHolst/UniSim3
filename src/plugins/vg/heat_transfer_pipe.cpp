/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "heat_transfer_pipe.h"

using namespace base;


namespace vg {

PUBLISH(HeatTransferPipe)

HeatTransferPipe::HeatTransferPipe(QString name, Box *parent)
    : HeatTransferLayerBase(name, parent)
{
    help("holds the radiative parameters of the heat pipes");
    Input(energyFluxRadiation).imports("actuators/heating[energyFluxRadiation]",CA);
    port("temperature")->imports("actuators/heating[temperature]",CA).help("For concistence. Not used here");
    port("area")->imports("construction/geometry[groundArea]",CA);
}

void HeatTransferPipe::reset() {
    swReflectivityBottom = swReflectivityTop =
    lwReflectivityBottom = lwReflectivityTop = 0.;
    swTransmissivityTop = swTransmissivityBottom =
    lwTransmissivityTop = lwTransmissivityBottom = 1.;
    updateAbsorptivities();
    update();
}

void HeatTransferPipe::update() {
    // Spread the flux equally upwards and downwards
    lwFluxDown = lwFluxUp = energyFluxRadiation/2.;
}

} //namespace

