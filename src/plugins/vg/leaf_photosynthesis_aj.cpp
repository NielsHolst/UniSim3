/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include <base/test_num.h>
#include <base/phys_math.h>
#include "arrhenius.h"
#include "leaf_photosynthesis_aj.h"

using namespace base;
using namespace phys_math;

namespace vg {
	
PUBLISH(LeafPhotosynthesisAj)

constexpr double
    Ea    = 29680,
    DS    = 631.88,
    DhD   = 200000,
    theta = 0.7;    // shape parameter (to compute J)

/*      Species     g0 RL25  Gs25 Jmax25
1  plantecophys 0.1000 0.92 42.75 210.00
2 Chrysanthemum 0.1060 0.29  115.67  -> the default values
3          Rose 0.0941 0.68 53.21 133.17
4        Tomato 0.1000 0.74 55.47 146.52
5         Wheat 0.2830 0.49 41.18 193.71
*/

LeafPhotosynthesisAj::LeafPhotosynthesisAj(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes leaf photosynthetic rate (CO2- and light-limited)");
    Input(gammaStar).imports("crop[gammaStar]",CA);
    Input(Jmax).imports("crop[Jmax]",CA);
    Input(k).imports("crop[k]",CA);
    Input(leafTemperature).imports("crop/temperature[value]",CA);
    Input(intercellularCo2).imports("../intercellularCo2[value]",CA);
    Input(parAbsorbed).imports("../parAbsorbed[value]",CA);
    Input(respiration).imports("../respiration[value]",CA);
    Output(JmaxTemp).help("Temperature-corrected Jmax").unit("μmol CO2/m2/s");
    Output(J).help("Light response at unlimited CO2").unit("μmol CO2/m2/s");
    Output(value).help("Net photosynthetic rate (Aj)").unit("μmol CO2/m2/s");
}

void LeafPhotosynthesisAj::setJmaxTemp() {
    JmaxTemp = arrhenius_peaked(Jmax, Ea, DS, DhD, leafTemperature);
}

void LeafPhotosynthesisAj::update() {
    setJmaxTemp();
    const double beta = 0.5;
    double
        a = k*beta*parAbsorbed + JmaxTemp,
        b = sqrt(a*a - 4*theta*k*beta*parAbsorbed*JmaxTemp),
        c = 2*theta;
    J = (a-b)/c;
    value = J * (intercellularCo2 - gammaStar) / (4.5*intercellularCo2 + 10.5*gammaStar) - respiration;
}

} //namespace

