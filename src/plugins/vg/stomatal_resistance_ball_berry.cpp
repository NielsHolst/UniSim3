/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "stomatal_resistance_ball_berry.h"

using std::max;
using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(StomatalResistanceBallBerry)


/*      Species     g0 RL25  Gs25 Jmax25
1  plantecophys 0.1000 0.92 42.75 210.00
2 Chrysanthemum 0.1060 0.29 48.95 115.67  -> the default values
3          Rose 0.0941 0.68 53.21 133.17
4        Tomato 0.1000 0.74 55.47 146.52
5         Wheat 0.2830 0.49 41.18 193.71
*/


StomatalResistanceBallBerry::StomatalResistanceBallBerry(QString name, Box *parent)
    : StomatalResistanceBase(name, parent)
{
    help("computes stomatal resistance after Ball et al. (1987)");
    Input(g0).imports("crop[ballBerryIntercept]",CA);
    Input(g1).imports("crop[ballBerrySlope]",CA);
    Input(leafPhotosynthesis).imports("../leafPhotosynthesis[An]",CA); // μmol CO2 / leaf m2 / s
    Input(rh).imports("indoors/humidity[rh]",CA).unit("[0;100]");
    Input(ca).imports("indoors/co2[value]",CA).unit("ppm");
}

double StomatalResistanceBallBerry::resetRsH2O() {
    return calcRsH2O(0.);
}

double StomatalResistanceBallBerry::updateRsH2O() {
    return calcRsH2O(leafPhotosynthesis);
}

double StomatalResistanceBallBerry::calcRsH2O(double An) {
    double g = g0 + g1*An*rh/100./ca;
    return 1./g;
}


} //namespace

