/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BOUNDARY_LAYER_RESISTANCE_JONES_A_H
#define BOUNDARY_LAYER_RESISTANCE_JONES_A_H

#include "boundary_layer_resistance_base.h"

namespace vg {

class BoundaryLayerResistanceJonesA : public BoundaryLayerResistanceBase
{
public:
    BoundaryLayerResistanceJonesA(QString name, Box *parent);
    void setRbH2O();
};
} //namespace


#endif
