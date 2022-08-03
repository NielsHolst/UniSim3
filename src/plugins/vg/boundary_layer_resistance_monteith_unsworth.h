/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BOUNDARY_LAYER_RESISTANCE_MONTEITH_UNSWORTH_H
#define BOUNDARY_LAYER_RESISTANCE_MONTEITH_UNSWORTH_H

#include "boundary_layer_resistance_base.h"

namespace vg {

class BoundaryLayerResistanceMonteithUnsworth : public BoundaryLayerResistanceBase
{
public:
    BoundaryLayerResistanceMonteithUnsworth(QString name, Box *parent);
    void reset();
    void setRbH2O();
private:
    // Inputs
    double leafTemperature, indoorsTemperature;
    // Outputs
    double gbH2Ofree, gbH2Oforced;
};

} //namespace


#endif
