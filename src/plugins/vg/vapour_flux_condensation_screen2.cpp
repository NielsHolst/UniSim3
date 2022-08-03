/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "vapour_flux_condensation_screen2.h"

using namespace base;

namespace vg {

PUBLISH(VapourFluxCondensationScreen2)

VapourFluxCondensationScreen2::VapourFluxCondensationScreen2(QString name, Box *parent)
    : VapourFluxCondensationScreenBase(name, parent, 2)
{
    help("models vapour flux by condensation on screen layer 2");
}

}
