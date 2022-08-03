/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/vector_op.h>
#include <base/publish.h>
#include "vapour_flux_condensation_screens.h"

using namespace base;
using vector_op::sum;

namespace vg {

PUBLISH(VapourFluxCondensationScreens)

VapourFluxCondensationScreens::VapourFluxCondensationScreens(QString name, Box *parent)
    : VapourFluxBase(name, parent)
{
    help("sums the vapour fluxes over all child boxes");
    Input(conductances).imports("./*[conductance]");
    Input(vapourFluxes).imports("./*[vapourFlux]");
    Input(gains).imports("./*[gain]");
}

void VapourFluxCondensationScreens::reset() {
    update();
}

void VapourFluxCondensationScreens::update() {
    conductance = sum(conductances);
    vapourFlux = sum(vapourFluxes);
    gain = sum(gains);
}

} //namespace

