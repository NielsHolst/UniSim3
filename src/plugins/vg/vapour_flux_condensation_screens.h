/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VAPOUR_FLUX_CONDENSATION_SCREENS_H
#define VAPOUR_FLUX_CONDENSATION_SCREENS_H

#include "vapour_flux_base.h"

namespace vg {

class VapourFluxCondensationScreens : public VapourFluxBase
{
public:
    VapourFluxCondensationScreens(QString name, Box *parent);
    void reset();
    void update();
private:
    // Outputs
    QVector<double> conductances, gains, vapourFluxes;
};
} //namespace


#endif
