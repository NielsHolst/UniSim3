/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "vapour_flux_transpiration.h"

using namespace base;

namespace vg {

PUBLISH(VapourFluxTranspiration)

VapourFluxTranspiration::VapourFluxTranspiration(QString name, Box *parent)
    : VapourFluxBase(name, parent)
{
    help("computes vapour flux by transpiration");
    port("conductance")->imports("crop/transpiration[conductance]",CA);
    port("vapourFlux")->imports("crop/transpiration[vapourFlux]",CA);
    port("gain")->imports("crop/transpiration[gain]",CA);
}

} //namespace

