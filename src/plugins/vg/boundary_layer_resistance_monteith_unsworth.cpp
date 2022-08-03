/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include "boundary_layer_resistance_monteith_unsworth.h"
#include <base/publish.h>

using namespace std;
using namespace base;

namespace vg {

PUBLISH(BoundaryLayerResistanceMonteithUnsworth)

BoundaryLayerResistanceMonteithUnsworth::BoundaryLayerResistanceMonteithUnsworth(QString name, Box *parent)
        : BoundaryLayerResistanceBase(name, parent)
{
    help("computes boundary layer resistance from Monteith and Unsworth (1990)");
    Input(leafTemperature).imports("../temperature[value]",CA).unit("oC");
    Input(indoorsTemperature).imports("indoors/temperature[value]",CA).unit("oC");
    Output(gbH2Oforced).help("Resistance depending on leaf wind speed").unit("s/m");
    Output(gbH2Ofree).help("Resistance depending on leaf temperature").unit("s/m");
}

void BoundaryLayerResistanceMonteithUnsworth::reset() {
    leafTemperature = indoorsTemperature;
    BoundaryLayerResistanceBase::reset();
}

void BoundaryLayerResistanceMonteithUnsworth::setRbH2O() {
    // Forced and free resistances are considered in parallel (i.e. as alternate pathways for water vapour)
    // Hence total resistance is found through addition of conductances
    double dTemp = fabs(leafTemperature - indoorsTemperature);
    gbH2Oforced = 1./( 300*sqrt(leafDimension/leafWindSpeed) );
    gbH2Ofree = 1./( 1000*pow(leafDimension/dTemp, 0.25) );
    rH2O = 1./(gbH2Oforced + gbH2Ofree);
}

} //namespace

