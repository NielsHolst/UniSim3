/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "ventilated_latent_heat_converter.h"

using namespace base;
using std::max;

namespace vg {

PUBLISH(VentilatedLatentHeatConverter)

VentilatedLatentHeatConverter::VentilatedLatentHeatConverter(QString name, Box *parent)
    : VapourFluxBase(name, parent)
{
    help("models a latent heat converter used for humidity reduction");
    Input(groundArea).imports("geometry[groundArea]").unit("m2");
    Input(indoorsAh).imports("indoors/humidity[ah]").unit("kg/m3");
    Input(volumeFlowRate).equals(2.5).help("Rate of air flow through the converter").unit("m3/s");
    // Additional outputs are needed to specify the flux of latent heat gain [W/m2]
}

void VentilatedLatentHeatConverter::update() {
    conductance = volumeFlowRate/groundArea;         // m/s = m3/s  / m2
    vapourFlux = conductance*(indoorsAh-convertedAh());    // kg/m2/s = m/s * kg/m3
    gain = conductance*convertedAh();              // kg/m2/s = m/s * kg/m3
}

double VentilatedLatentHeatConverter::dewRate() {
    // NB. Needs code for interpolated value...
    return 0.004; // kg/s
}

double VentilatedLatentHeatConverter::convertedAh() {
    return indoorsAh - dewRate()/volumeFlowRate; // kg/m3 = kg/s / m3/s
}

} //namespace

