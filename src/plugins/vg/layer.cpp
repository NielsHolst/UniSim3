/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/exception.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "layer.h"

using namespace base;
using namespace TestNum;
using phys_math::infinity;

#define CHECK_PARAM(X) checkParameter(#X, X)
#define CHECK_PARAM_SUM(X) checkParameterSum(#X, X)
#define CHECK_ABS(X) checkAbsorptivity(#X, X)
#define CHECK_REFL(X) checkReflectivity(#X, X)
namespace vg {

PUBLISH(Layer)

Layer::Layer(QString name, Box *parent)
    : Box(name, parent),
      LayerParameters()
{
    help("holds radiative and heat parameters of a layer");
    Input(checkBoundaries).equals(false).unit("bool").help("Check parameter boundaries?");
}

void Layer::useLayerAsInput() {
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

    // When used as input these ports are per layer area
    Input(Utop).unit("W/K/m2 layer").help("Heat transfer coefficient at the top");
    Input(Ubottom).unit("W/K/m2 layer").help("Heat transfer coefficient at the bottom");
    Input(heatCapacity).help("Area-specific heat capacity").unit("J/K/m2 layer");
    makeTransparent();
}

void Layer::useLayerAsOutput() {
    Output(swAbsorptivityTop).unit("[0;1]").help("Short-wave absorptivity at the top");
    Output(swReflectivityTop).unit("[0;1]").help("Short-wave reflectivity at the top");
    Output(swTransmissivityTop).equals(1.).unit("[0;1]").help("Short-wave transmissivity at the top");

    Output(swAbsorptivityBottom).unit("[0;1]").help("Short-wave absorptivity at the bottom");
    Output(swReflectivityBottom).unit("[0;1]").help("Short-wave reflectivity at the bottom");
    Output(swTransmissivityBottom).equals(1.).unit("[0;1]").help("Short-wave transmissivity at the bottom");

    Output(lwAbsorptivityTop).unit("[0;1]").help("Long-wave absorptivity at the top");
    Output(lwReflectivityTop).unit("[0;1]").help("Long-wave reflectivity at the top");
    Output(lwTransmissivityTop).equals(1.).unit("[0;1]").help("Long-wave transmissivity at the top");

    Output(lwAbsorptivityBottom).unit("[0;1]").help("Long-wave absorptivity at the bottom");
    Output(lwReflectivityBottom).unit("[0;1]").help("Long-wave reflectivity at the bottom");
    Output(lwTransmissivityBottom).equals(1.).unit("[0;1]").help("Long-wave transmissivity at the bottom");

    // When used as output these ports are per ground area
    Output(Utop).unit("W/K/m2 ground").help("Heat transfer coefficient at the top");
    Output(Ubottom).unit("W/K/m2 ground").help("Heat transfer coefficient at the bottom");
    Output(heatCapacity).unit("J/K/m2 ground").help("Area-specific heat capacity");
    makeTransparent();
}

void Layer::makeTransparent() {
    swReflectivityTop   = swReflectivityBottom   = 0.;
    swTransmissivityTop = swTransmissivityBottom = 1.;
    lwReflectivityTop   = lwReflectivityBottom   = 0.;
    lwTransmissivityTop = lwTransmissivityBottom = 1.;
    swAbsorptivityTop   = swAbsorptivityBottom   = 0.;
    lwAbsorptivityTop   = lwAbsorptivityBottom   = 0.;
    Utop = Ubottom =
    heatCapacity = 0.;
}

void Layer::checkParameters() const {
    CHECK_PARAM(swAbsorptivityTop);
    CHECK_PARAM(swReflectivityTop);
    CHECK_PARAM(swTransmissivityTop);

    CHECK_PARAM(swAbsorptivityBottom);
    CHECK_PARAM(swReflectivityBottom);
    CHECK_PARAM(swTransmissivityBottom);

    CHECK_PARAM(lwAbsorptivityTop);
    CHECK_PARAM(lwReflectivityTop);
    CHECK_PARAM(lwTransmissivityTop);

    CHECK_PARAM(lwAbsorptivityBottom);
    CHECK_PARAM(lwReflectivityBottom);
    CHECK_PARAM(lwTransmissivityBottom);

    CHECK_PARAM_SUM(swReflectivityTop+swTransmissivityTop+swAbsorptivityTop);
    CHECK_PARAM_SUM(lwReflectivityTop+lwTransmissivityTop+lwAbsorptivityTop);
    CHECK_PARAM_SUM(swReflectivityBottom+swTransmissivityBottom+swAbsorptivityBottom);
    CHECK_PARAM_SUM(lwReflectivityBottom+lwTransmissivityBottom+lwAbsorptivityBottom);

    if ((Utop == 0. || Ubottom == 0) && heatCapacity > 0)
        ThrowException("U-value can be zero only if heat capacity is zero too").value(heatCapacity).context(this);

    if (checkBoundaries)
        checkParameterBoundaries();
}

void Layer::checkParameterBoundaries() const {
    CHECK_ABS(swAbsorptivityTop);
    CHECK_ABS(lwAbsorptivityTop);
    CHECK_ABS(swAbsorptivityBottom);
    CHECK_ABS(lwAbsorptivityBottom);

    CHECK_ABS(swReflectivityTop);
    CHECK_ABS(lwReflectivityTop);
    CHECK_ABS(swReflectivityBottom);
    CHECK_ABS(lwReflectivityBottom);
}

void Layer::checkParameter(QString name, double value) const {
    if (ltZero(value) || gt(value, 1.))
        ThrowException("Radiative parameter out of bounds").value(value).hint(name).context(this);
}

void Layer::checkParameterSum(QString name, double value) const {
    if (ne(value, 1.))
        ThrowException("Radiative parameters out of bounds").value(value).hint(name).context(this);
}

void Layer::checkAbsorptivity(QString name, double value) const {
    if (lt(value, 0.05))
        ThrowException("Absorptivity must be >= 0.05").value(value).hint(name).context(this);
}

void Layer::checkReflectivity(QString name, double value) const {
    if (lt(value, 0.03) || gt(value, 0.95))
        ThrowException("Reflectivity must be between 0.03 and 0.95").value(value).hint(name).context(this);
}

} //namespace

