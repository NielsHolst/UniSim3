/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "layer.h"

#define CHK(x) checkRange(x, #x)

using namespace base;
using namespace phys_math;
using namespace TestNum;

namespace vg {

PUBLISH(Layer)

Layer::Layer(QString name, Box *parent)
    : Box(name, parent)
{
    help("holds radiative and heat parameters of a layer");
    Input(swReflectivityTop).unit("[0;1]").help("Short-wave reflectivity at the top");
    Input(swReflectivityBottom).unit("[0;1]").help("Short-wave reflectivity at the bottom");
    Input(swTransmissivityTop).equals(1.).unit("[0;1]").help("Short-wave transmissivity at the top");
    Input(swTransmissivityBottom).equals(1.).unit("[0;1]").help("Short-wave transmissivity at the bottom");

    Input(lwReflectivityTop).unit("[0;1]").help("Long-wave reflectivity at the top");
    Input(lwReflectivityBottom).unit("[0;1]").help("Long-wave reflectivity at the bottom");
    Input(lwTransmissivityTop).equals(1.).unit("[0;1]").help("Long-wave transmissivity at the top");
    Input(lwTransmissivityBottom).equals(1.).unit("[0;1]").help("Long-wave transmissivity at the bottom");

    Input(Utop).equals(infinity()).unit("W/K/m2 layer").help("Heat transfer coefficient at the top");
    Input(Ubottom).equals(infinity()).unit("W/K/m2 layer").help("Heat transfer coefficient at the bottom");
    Input(heatCapacity).help("Area-specific heat capacity").unit("J/K/m2 layer");

    Output(swAbsorptivityTop).unit("[0;1]").help("Short-wave absorptivity at the top");
    Output(swAbsorptivityBottom).unit("[0;1]").help("Short-wave absorptivity at the bottom");
    Output(lwAbsorptivityTop).unit("[0;1]").help("Long-wave absorptivity at the top");
    Output(lwAbsorptivityBottom).unit("[0;1]").help("Long-wave absorptivity at the bottom");
}

void Layer::reset() {
    updateAbsorptivities();
}

void Layer::update() {
    updateAbsorptivities();
}

void Layer::updateAbsorptivities() {
    swAbsorptivityTop = 1. - swReflectivityTop - swTransmissivityTop;
    swAbsorptivityBottom  = 1. - swReflectivityBottom  - swTransmissivityBottom;
    lwAbsorptivityTop = 1. - lwReflectivityTop - lwTransmissivityTop;
    lwAbsorptivityBottom  = 1. - lwReflectivityBottom  - lwTransmissivityBottom;

    snapTo(swAbsorptivityTop, 0.); snapTo(swAbsorptivityTop, 1.);
    snapTo(swAbsorptivityBottom , 0.); snapTo(swAbsorptivityBottom , 1.);
    snapTo(lwAbsorptivityTop, 0.); snapTo(lwAbsorptivityTop, 1.);
    snapTo(lwAbsorptivityBottom , 0.); snapTo(lwAbsorptivityBottom , 1.);

    CHK(swReflectivityTop);
    CHK(swReflectivityBottom);
    CHK(swTransmissivityTop);
    CHK(swTransmissivityBottom);
    CHK(lwReflectivityTop);
    CHK(lwReflectivityBottom);
    CHK(lwTransmissivityTop);
    CHK(lwTransmissivityBottom);
    CHK(swAbsorptivityTop);
    CHK(swAbsorptivityBottom);
    CHK(lwAbsorptivityTop);
    CHK(lwAbsorptivityBottom);
}

void Layer::checkRange(double x, QString name) const {
    if (TestNum::ltZero(x) || TestNum::gt(x, 1.))
        ThrowException("Radiative parameter must be inside [0;1]").
                value(name).value2(x).context(this);
}

} //namespace

