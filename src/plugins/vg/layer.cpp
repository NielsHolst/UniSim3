/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "layer.h"

using namespace base;
using namespace TestNum;

#define CHECK_INPUT(X) checkInput(#X, X)
#define CHECK_INPUT_SUM(X) checkInputSum(#X, X)

namespace vg {

PUBLISH(Layer)

Layer::Layer(QString name, Box *parent)
    : Box(name, parent)
{
    help("holds radiative and heat parameters of a layer");
    Input(swAbsorptivityTop).unit("[0;1]").help("Short-wave absorptivity at the top");
    Input(swReflectivityTop).unit("[0;1]").help("Short-wave reflectivity at the top");
    Input(swTransmissivityTop).equals(1.).unit("[0;1]").help("Short-wave transmissivity at the top");

    Input(swAbsorptivityBottom).unit("[0;1]").help("Short-wave absorptivity at the bottom");
    Input(swReflectivityBottom).unit("[0;1]").help("Short-wave reflectivity at the bottom");
    Input(swTransmissivityBottom).equals(1.).unit("[0;1]").help("Short-wave transmissivity at the bottom");

    Input(lwAbsorptivityTop).unit("[0;1]").help("Long-wave absorptivity at the top");
    Input(lwReflectivityTop).unit("[0;1]").help("Long-wave reflectivity at the top");
    Input(lwTransmissivityTop).equals(1.).unit("[0;1]").help("Long-wave transmissivity at the top");

    Input(lwAbsorptivityBottom).unit("[0;1]").help("Long-wave absorptivity at the bottom");
    Input(lwReflectivityBottom).unit("[0;1]").help("Long-wave reflectivity at the bottom");
    Input(lwTransmissivityBottom).equals(1.).unit("[0;1]").help("Long-wave transmissivity at the bottom");

    Input(Utop).unit("W/K/m2 layer").help("Heat transfer coefficient at the top");
    Input(Ubottom).unit("W/K/m2 layer").help("Heat transfer coefficient at the bottom");
    Input(heatCapacity).help("Area-specific heat capacity").unit("J/K/m2 layer");
}

void Layer::update() {
    checkInputs();
}

void Layer::checkInputs() {
    CHECK_INPUT(swAbsorptivityTop);
    CHECK_INPUT(swReflectivityTop);
    CHECK_INPUT(swTransmissivityTop);

    CHECK_INPUT(swAbsorptivityBottom);
    CHECK_INPUT(swReflectivityBottom);
    CHECK_INPUT(swTransmissivityBottom);

    CHECK_INPUT(lwAbsorptivityTop);
    CHECK_INPUT(lwReflectivityTop);
    CHECK_INPUT(lwTransmissivityTop);

    CHECK_INPUT(lwAbsorptivityBottom);
    CHECK_INPUT(lwReflectivityBottom);
    CHECK_INPUT(lwTransmissivityBottom);

    CHECK_INPUT_SUM(swReflectivityTop+swTransmissivityTop+swAbsorptivityTop);
    CHECK_INPUT_SUM(lwReflectivityTop+lwTransmissivityTop+lwAbsorptivityTop);
    CHECK_INPUT_SUM(swReflectivityBottom+swTransmissivityBottom+swAbsorptivityBottom);
    CHECK_INPUT_SUM(lwReflectivityBottom+lwTransmissivityBottom+lwAbsorptivityBottom);
}

void Layer::checkInput(QString name, double value) {
    if (ltZero(value) || gt(value, 1.))
        ThrowException("Radiative parameter out of bounds").value(value).hint(name).context(this);
}

void Layer::checkInputSum(QString name, double value) {
    if (ne(value, 1.))
        ThrowException("Radiative parameters out of bounds").value(value).hint(name).context(this);
}

} //namespace

