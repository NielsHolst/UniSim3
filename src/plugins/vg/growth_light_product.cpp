/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "growth_light_product.h"
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>

using namespace base;

namespace vg {

PUBLISH(GrowthLightProduct)

GrowthLightProduct::GrowthLightProduct(QString name, Box *parent)
    : Box(name, parent)
{
    help("holds the parameters for a lamp product");
    Input(power).help("Power per lamp").unit("W");
    Input(ballast).help("Ballast power per lamp").unit("W");
    Input(parPhotonCoef).equals(1.6).help("Number of PAR photons per spent lamp energy").unit("micromole PAR/J");
    Input(propSw).equals(0.5).help("Proportion of power emitted as short-wave radiation").unit("[0;1]");
    Input(propLw).equals(0.1).help("Proportion of power emitted as long-wave radiation").unit("[0;1]");
    Input(propConv).equals(0.4).help("Proportion of power emitted as convective heat").unit("[0;1]");
    Input(propBallastLw).equals(0.7).help("Proportion of ballast emitted as long-wave radiation").unit("[0;1]");
    Input(propBallastConv).equals(0.3).help("Proportion of ballast emitted as convective heat").unit("[0;1]");
    Input(minPeriodOn).help("Minimum period that light stays on").unit("min");
}


} //namespace

