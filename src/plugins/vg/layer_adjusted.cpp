/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/test_num.h>
#include "layer_adjusted.h"

using namespace base;
using namespace TestNum;

#define CHECK_OUTPUT(X) checkOutput(#X, X)
#define CHECK_OUTPUT_SUM(X) checkOutputSum(#X, X)

namespace vg {

PUBLISH(LayerAdjusted)

LayerAdjusted::LayerAdjusted(QString name, Box *parent)
    : Box(name, parent)
{
    help("holds adjusted radiative and heat parameters of a layer");
    Input(coverPerGroundArea).imports("gh/geometry[coverPerGroundArea]");

    const char text[] = "Parameter adjusted for chalk and state";
    Output(swAbsorptivityTopAdj).unit("[0;1]").help(text);
    Output(swReflectivityTopAdj).unit("[0;1]").help(text);
    Output(swTransmissivityTopAdj).unit("[0;1]").help(text);

    Output(swAbsorptivityBottomAdj).unit("[0;1]").help(text);
    Output(swReflectivityBottomAdj).unit("[0;1]").help(text);
    Output(swTransmissivityBottomAdj).unit("[0;1]").help(text);

    Output(lwAbsorptivityTopAdj).unit("[0;1]").help(text);
    Output(lwReflectivityTopAdj).unit("[0;1]").help(text);
    Output(lwTransmissivityTopAdj).unit("[0;1]").help(text);

    Output(lwAbsorptivityBottomAdj).unit("[0;1]").help(text);
    Output(lwReflectivityBottomAdj).unit("[0;1]").help(text);
    Output(lwTransmissivityBottomAdj).unit("[0;1]").help(text);

    Output(UtopAdj).help(text);
    Output(UbottomAdj).help(text);
    Output(heatCapacityAdj).help(text);
}

void LayerAdjusted::update() {
    checkOutputs();
}

void LayerAdjusted::makeTransparent() {
    swReflectivityTopAdj   = swReflectivityBottomAdj   = 0.;
    swTransmissivityTopAdj = swTransmissivityBottomAdj = 1.;
    lwReflectivityTopAdj   = lwReflectivityBottomAdj   = 0.;
    lwTransmissivityTopAdj = lwTransmissivityBottomAdj = 1.;
    swAbsorptivityTopAdj   = swAbsorptivityBottomAdj   = 0.;
    lwAbsorptivityTopAdj   = lwAbsorptivityBottomAdj   = 0.;
    UtopAdj = UbottomAdj =
    heatCapacityAdj = 0.;

}

void LayerAdjusted::checkOutputs() {
    CHECK_OUTPUT(swAbsorptivityTopAdj);
    CHECK_OUTPUT(swReflectivityTopAdj);
    CHECK_OUTPUT(swTransmissivityTopAdj);

    CHECK_OUTPUT(swAbsorptivityBottomAdj);
    CHECK_OUTPUT(swReflectivityBottomAdj);
    CHECK_OUTPUT(swTransmissivityBottomAdj);

    CHECK_OUTPUT(lwAbsorptivityTopAdj);
    CHECK_OUTPUT(lwReflectivityTopAdj);
    CHECK_OUTPUT(lwTransmissivityTopAdj);

    CHECK_OUTPUT(lwAbsorptivityBottomAdj);
    CHECK_OUTPUT(lwReflectivityBottomAdj);
    CHECK_OUTPUT(lwTransmissivityBottomAdj);

    CHECK_OUTPUT_SUM(swReflectivityTopAdj+swTransmissivityTopAdj+swAbsorptivityTopAdj);
    CHECK_OUTPUT_SUM(lwReflectivityTopAdj+lwTransmissivityTopAdj+lwAbsorptivityTopAdj);
    CHECK_OUTPUT_SUM(swReflectivityBottomAdj+swTransmissivityBottomAdj+swAbsorptivityBottomAdj);
    CHECK_OUTPUT_SUM(lwReflectivityBottomAdj+lwTransmissivityBottomAdj+lwAbsorptivityBottomAdj);
}

void LayerAdjusted::checkOutput(QString name, double value) {
    if (ltZero(value) || gt(value, 1.))
        ThrowException("Radiative parameter out of bounds").value(value).hint(name).context(this);
}

void LayerAdjusted::checkOutputSum(QString name, double value) {
    if (ne(value, 1.))
        ThrowException("Radiative parameters out of bounds").value(value).hint(name).context(this);
}

} //namespace

