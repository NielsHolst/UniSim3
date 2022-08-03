/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VAPOUR_FLUX_CONDENSATION_SCREEN3_H
#define VAPOUR_FLUX_CONDENSATION_SCREEN3_H

#include "vapour_flux_condensation_screen_base.h"

namespace vg {

class VapourFluxCondensationScreen3 : public VapourFluxCondensationScreenBase
{
public:
    VapourFluxCondensationScreen3(QString name, Box *parent);
};
} //namespace


#endif
