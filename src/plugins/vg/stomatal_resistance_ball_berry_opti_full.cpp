/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "stomatal_resistance_ball_berry_opti_full.h"

using std::max;
using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(StomatalResistanceBallBerryOptiFull)

/*         Crop          g0       g1       gk
          wheat       0.343   -1.332   10.158
*/

StomatalResistanceBallBerryOptiFull::StomatalResistanceBallBerryOptiFull(QString name, Box *parent)
    : StomatalResistanceBase(name, parent)
{
    help("computes stomatal resistance after Ball et al. (1987), Medlyn et al. (2011) and Duursma et al. (2013)");
    Input(g0).equals(0.343).help("Minimum stomatal conductance for CO2");
    Input(g1).equals(-1.332).help("Slope of stomatal conductance");
    Input(gk).equals(10.158).help("Shape parameter");
    Input(rh).imports("indoors/humidity[rh]",CA).unit("[0;100]");
    Input(co2).imports("indoors/co2[value]",CA).unit("ppm");
    Input(temperature).imports("indoors/temperature[value]",CA);
    Input(Pg).imports("../photosynthesis[Pg]",CA);
}

double StomatalResistanceBallBerryOptiFull::resetRsH2O() {
    return calcRsH2O(700.);
}

double StomatalResistanceBallBerryOptiFull::updateRsH2O() {
    return calcRsH2O(co2);
}

/*  Units
 *  g: mol CO2 / m2 / s
 * Pg: mg  CO2 / m2 / s
 * 1 mol CO2 = 44.01 g = 0.0441 mg
*/

double StomatalResistanceBallBerryOptiFull::calcRsH2O(double ca) {
    double vpd = vpdFromRh(temperature, rh),
           g = g0 + Pg/0.0441*(1 + g1/pow(vpd, (1 - gk)))/ca;
    return 1./g;
}


} //namespace

