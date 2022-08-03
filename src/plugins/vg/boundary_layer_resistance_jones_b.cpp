/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include "boundary_layer_resistance_jones_b.h"

using std::max;
using std::min;
using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(BoundaryLayerResistanceJonesB)

BoundaryLayerResistanceJonesB::BoundaryLayerResistanceJonesB(QString name, Box *parent)
    : BoundaryLayerResistanceBase(name, parent)
{
    help("computes boundary layer resistance from Jones (1992, eq. A.8.1)");
    Input(evaporation).imports("../transpiration[vapourFlux]",CA).unit("kg/m2/s");
    Input(leafTemperature).imports("../temperature[value]",CA).unit("oC");
    Input(indoorsTemperature).imports("indoors/temperature[value]",CA).unit("oC");
    Input(indoorsRh).imports("indoors/humidity[rh]",CA).unit("[0;100]");
}

void BoundaryLayerResistanceJonesB::reset() {
    leafTemperature = indoorsTemperature;
    evaporation = 1e-6;
    BoundaryLayerResistanceBase::reset();
}

void BoundaryLayerResistanceJonesB::setRbH2O() {
    double svpLeaf = svp(leafTemperature),
           vpAir = vpFromRh(indoorsTemperature, indoorsRh),
           vpd = max(svpLeaf - vpAir, 0.),
           gbH2O = max(1000.*evaporation/( 0.622*RhoAir/P0*vpd ), 0.001);

    rH2O = 1./gbH2O;
    if (std::isnan(rH2O)) {
        ThrowException("BoundaryLayerResistanceJonesB is not a number").context(this);
    }
}

} //namespace

