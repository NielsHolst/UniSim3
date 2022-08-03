/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/

#include "boundary_layer_resistance_base.h"
#include <base/publish.h>

using namespace base;

namespace vg {

BoundaryLayerResistanceBase::BoundaryLayerResistanceBase(QString name, Box *parent)
    : Box(name, parent)
{
    setClassName("vg", "BoundaryLayerResistanceBase");
    help("base class to compute bounday layer resistance");
    Input(leafDimension).equals(25./1000.).help("Description of leaf geometry").unit("mm");
    Input(leafWindSpeed).imports("../leafWindSpeed[value]",CA).unit("oC");
    Output(rH2O).help("The boundary layer resistance against water vapour").unit("s/m2");
    Output(rCo2).help("The boundary layer resistance against CO2").unit("s/m2");
}

void BoundaryLayerResistanceBase::reset() {
    update();
}

void BoundaryLayerResistanceBase::update() {
    setRbH2O();
    rCo2 = rH2O*1.37;
}

} //namespace
