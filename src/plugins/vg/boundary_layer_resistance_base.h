/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BOUNDARY_LAYER_RESISTANCE_BASE_H
#define BOUNDARY_LAYER_RESISTANCE_BASE_H

#include <base/box.h>

namespace vg {

class BoundaryLayerResistanceBase : public base::Box
{
public:
    BoundaryLayerResistanceBase(QString name, Box *parent);
    void reset();
    void update() final;
protected:
    // Inputs
    double leafDimension, leafWindSpeed;
    // Outputs
    double rH2O, rCo2;
private:
    // Methods
    virtual void setRbH2O() = 0;
};

} //namespace

#endif
