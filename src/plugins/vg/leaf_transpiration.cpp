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
#include "leaf_transpiration.h"
#include "vapour_flux_base.h"

using std::max;
using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(LeafTranspiration)

LeafTranspiration::LeafTranspiration(QString name, Box *parent)
    : VapourFluxBase(name, parent)
{
    help("models leaf transpiration");
    Input(lai).imports("..[lai]",CA);
    Input(cropCoverage).imports("crop[coverage]",CA);
    Input(indoorsAh).imports("indoors/humidity[ah]",CA).unit("kg/m3");
    Input(radiationAbsorbed).imports("energyBudget/crop[absorbed]",CA).unit("W/m2");
    Input(leafTemperature).imports("../temperature[value]",CA).unit("oC");
    Input(rsH2O).computes("mean(../photosynthesis/*/stomatalResistance[rH2O])").unit("s/m2");
    Input(rbH2O).imports("../boundaryLayerResistance[rH2O]").unit("s/m2");
    Output(leafAh).help("Leaf absolute humidity").unit("kg/m3");
}

void LeafTranspiration::reset() {
    leafAh = indoorsAh;
}

void LeafTranspiration::update() {
    if (lai == 0.) {
        conductance = vapourFlux = gain = 0.;
    }
    else {
        double laiNet = lai*cropCoverage,
               s = svpSlope(leafTemperature);
        conductance = 2*laiNet/((1 + s/Psychr)*rbH2O + rsH2O);
        leafAh = sah(leafTemperature) + s/Psychr*rbH2O/2/laiNet*radiationAbsorbed/LHe;
        vapourFlux = max(conductance*(leafAh - indoorsAh), 0.);
        gain = conductance*leafAh;
    }
}


} //namespace

