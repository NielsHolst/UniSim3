/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "average_material.h"

using namespace base;
using namespace vector_op;

namespace vg {

PUBLISH(AverageMaterial)

AverageMaterial::AverageMaterial(QString name, Box *parent)
    : Box(name, parent),
      Layer(name, parent),
      LayerAdjusted(name, parent)
{
    help("averages material by area and weight");
    Input(myAreas);
    Input(myWeights);
    Input(allAreas).imports("shelter/faces/*[area]");
    Input(allWeights).imports("shelter/faces/*[weight]");
}

void AverageMaterial::reset() {
    checkInputs();

    if (allAreas.size() != 6)
        ThrowException("shelter/faces/*[area] must match 6 ports").
                value(allAreas.size()).context(this);
    if (allWeights.size() != 6)
        ThrowException("shelter/faces/*[weight] must match 6 ports").
                value(allAreas.size()).context(this);

    double sumAllWeightAreas = sumOfProducts(allAreas,allWeights),
           sumAllAreas = sum(allAreas);
   _propWeightArea = (sumAllWeightAreas==0.) ? 0. : sumOfProducts(myAreas,myWeights)/sumAllWeightAreas;
   _propArea = (sumAllAreas==0.) ? 0. : sum(myAreas)/sumAllAreas;

    update();
}

#define ADJ_BY_AREA_WEIGHTS(X) X##Adj = X*_propWeightArea
#define ADJ_BY_AREAS(X)        X##Adj = X*_propArea

void AverageMaterial::update() {
    ADJ_BY_AREA_WEIGHTS(swReflectivityTop);
    ADJ_BY_AREA_WEIGHTS(swReflectivityBottom);
    ADJ_BY_AREA_WEIGHTS(lwReflectivityTop);
    ADJ_BY_AREA_WEIGHTS(lwReflectivityBottom);
    ADJ_BY_AREA_WEIGHTS(swTransmissivityTop);
    ADJ_BY_AREA_WEIGHTS(swTransmissivityBottom);
    ADJ_BY_AREA_WEIGHTS(lwTransmissivityTop);
    ADJ_BY_AREA_WEIGHTS(lwTransmissivityBottom);
    ADJ_BY_AREA_WEIGHTS(swAbsorptivityTop);
    ADJ_BY_AREA_WEIGHTS(swAbsorptivityBottom);
    ADJ_BY_AREA_WEIGHTS(lwAbsorptivityTop);
    ADJ_BY_AREA_WEIGHTS(lwAbsorptivityBottom);
    ADJ_BY_AREAS       (Utop);
    ADJ_BY_AREAS       (Ubottom);
    ADJ_BY_AREAS       (heatCapacity);
}

} //namespace


