/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VAPOUR_FLUX_BASE_H
#define VAPOUR_FLUX_BASE_H

#include <base/box.h>

namespace vg {

class VapourFluxBase : public base::Box
{
public:
    VapourFluxBase(QString name, Box *parent);
protected:
    // Outputs
    double conductance, gain, vapourFlux;
};
} //namespace


#endif
