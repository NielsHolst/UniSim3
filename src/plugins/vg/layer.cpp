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

Layer::Layer(QString name, Box *parent)
    : Box(name, parent),
      LayerParameters()
{
    help("holds radiative and heat parameters of a layer");
    Input(checkBoundaries).equals(false).unit("bool").help("Check parameter boundaries?");
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

