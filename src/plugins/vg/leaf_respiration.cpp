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
#include "leaf_respiration.h"

using std::max;
using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(LeafRespiration)

/*      Species     g0 RL25  Gs25 Jmax25
1  plantecophys 0.1000 0.92 42.75 210.00
2 Chrysanthemum 0.1060 0.29 48.95 115.67  -> the default values
3          Rose 0.0941 0.68 53.21 133.17
4        Tomato 0.1000 0.74 55.47 146.52
5         Wheat 0.2830 0.49 41.18 193.71
*/

LeafRespiration::LeafRespiration(QString name, Box *parent)
    : Box(name, parent)
{
    help("models leaf light and dark respiration");
    Input(leafTemperature).imports("crop/temperature[value]",CA).unit("oC");
    Input(canopyParAbsorbed).imports("energyBudget/crop[parAbsorbed]",CA);
    Input(RL).imports("crop[lightRespiration]",CA);
    Input(darknessThreshold).equals(5.).help("Threshold for dark respiration").unit("μmol CO2/m2 ground/s");
    Input(Q10).equals(1.92).help("Q10 value");
    Output(value).help("Respiration").unit("μmol CO2/m2/s");
}

void LeafRespiration::reset() {
    value = RL;
}

void LeafRespiration::update() {
    value = RL*pow(Q10, (leafTemperature-25.)/10.);
    // Dark respiration is roughly twice light respiration
    if (canopyParAbsorbed < darknessThreshold)
        value *= 2.;
}


} //namespace

