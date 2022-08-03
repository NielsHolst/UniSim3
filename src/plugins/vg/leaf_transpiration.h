/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LEAF_TRANSPIRATION_H
#define LEAF_TRANSPIRATION_H

#include <base/box.h>
#include "vapour_flux_base.h"

namespace vg {

class LeafTranspiration : public VapourFluxBase
{
public:
    LeafTranspiration(QString name, Box *parent);
    void reset();
    void update();

private:
    // Inputs
    double radiationAbsorbed, rsH2O, rbH2O,
        indoorsAh, leafTemperature, lai, cropCoverage;
    // Outputs
    double leafAh;
};
} //namespace


#endif
