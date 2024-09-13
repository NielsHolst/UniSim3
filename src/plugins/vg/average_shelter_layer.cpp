/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/test_num.h>
#include <base/vector_op.h>
#include "average_shelter_layer.h"
#include "face.h"

using namespace base;
using TestNum::eq;
using vector_op::sum;
using vector_op::sumOfProducts;

namespace vg {

AverageShelterLayer::AverageShelterLayer(QString name, Box *parent)
    : LayerAsOutput(name, parent)
{
    help("computes average parameters for covers and screens");
    Input(areas).imports("shelter/faces/*[area]");
    Input(swWeights).imports("shelter/faces/*[swWeight]");
    Input(lwWeights).imports("shelter/faces/*[lwWeight]");
    Input(groundArea).imports("gh/geometry[groundArea]");
    Input(coverArea).imports("gh/geometry[coverArea]");
    port("Utop")->unit("W/K/m2 ground");
    port("Ubottom")->unit("W/K/m2 ground");
    port("heatCapacity")->unit("J/K/m2 ground");
}

void AverageShelterLayer::initialize() {
    _faces = findMany<Face*>("shelter/faces/*");
    if (_faces.size() != 6)
        ThrowException("Expected six faces").value("\n" + fullNames(_faces).join("\n")).context(this);

    Q_ASSERT(areas.size() == 6);
    Q_ASSERT(swWeights.size() == 6);
    Q_ASSERT(lwWeights.size() == 6);
    _areaWeights.resize(6);
    _areaPerGround.resize(6);
    _swWeightsTop.resize(6);
    _lwWeightsTop.resize(6);

    double
        lwWeightsTopSum = sumOfProducts(areas, lwWeights);
    for (int i=0; i<6; ++i) {
        _lwWeightsTop[i] = lwWeights.at(i)*areas.at(i)/lwWeightsTopSum;
    }
    double
        sum3 = sum(_lwWeightsTop);
    Q_ASSERT(eq(sum(_lwWeightsTop), 1.));
}

//namespace {
//    void scaleToOne(double &a, double &b, double &c) {
//        const double sum = a + b + c;
//        a /= sum;
//        b /= sum;
//        c /= sum;
//    }
//}

void AverageShelterLayer::updateParameters(const int layerIndex, const QVector<double> &adjustments) {
    double
        swWeightsTopSum = sumOfProducts(areas, swWeights);
    for (int i=0; i<6; ++i) {
        _swWeightsTop[i] = swWeights.at(i)*areas.at(i)/swWeightsTopSum;
        _areaWeights[i] = areas.at(i)/coverArea;
        _areaPerGround[i] = areas.at(i)/groundArea;
    }
//    double
//        sum1 = sum(areas),
//        sum2 = sum(_swWeightsTop),
//        sum4 = sum(_areaWeights),
//        sum5 = sum(_areaPerGround);
    Q_ASSERT(eq(sum(areas), coverArea));
    Q_ASSERT(eq(sum(_swWeightsTop), 1.));
    Q_ASSERT(eq(sum(_areaWeights), 1.));
    Q_ASSERT(eq(sum(_areaPerGround), coverArea/groundArea));



    // Compute Layer parameter values weighed over all six surfaces
    swReflectivityTop = swReflectivityBottom = swTransmissivityTop = swTransmissivityBottom = swAbsorptivityTop = swAbsorptivityBottom =
    lwReflectivityTop = lwReflectivityBottom = lwTransmissivityTop = lwTransmissivityBottom = lwAbsorptivityTop = lwAbsorptivityBottom =
    heatCapacity = Utop = Ubottom = 0.;

    int i = 0;
    for (Face *face : _faces) {
        LayerParameters p = transform(face->parameters(layerIndex), adjustments);
        swReflectivityTop   += _swWeightsTop.at(i)*p.swReflectivityTop;
        swTransmissivityTop += _swWeightsTop.at(i)*p.swTransmissivityTop;
        swAbsorptivityTop   += _swWeightsTop.at(i)*p.swAbsorptivityTop;

        lwReflectivityTop   += _lwWeightsTop.at(i)*p.lwReflectivityTop;
        lwTransmissivityTop += _lwWeightsTop.at(i)*p.lwTransmissivityTop;
        lwAbsorptivityTop   += _lwWeightsTop.at(i)*p.lwAbsorptivityTop;

        swReflectivityBottom   += _areaWeights.at(i)*p.swReflectivityBottom;
        swTransmissivityBottom += _areaWeights.at(i)*p.swTransmissivityBottom;
        swAbsorptivityBottom   += _areaWeights.at(i)*p.swAbsorptivityBottom;

        lwReflectivityBottom   += _areaWeights.at(i)*p.lwReflectivityBottom;
        lwTransmissivityBottom += _areaWeights.at(i)*p.lwTransmissivityBottom;
        lwAbsorptivityBottom   += _areaWeights.at(i)*p.lwAbsorptivityBottom;

        // heatCapacity, UTop and UBottom are converted to [W/K/m2 ground]
        heatCapacity += _areaPerGround.at(i)*p.heatCapacity;
        Utop         += _areaPerGround.at(i)*p.Utop;
        Ubottom      += _areaPerGround.at(i)*p.Ubottom;
        ++i;
    }
    double
            sum1 = swReflectivityTop + swTransmissivityTop + swAbsorptivityTop,
            sum2 = lwReflectivityTop + lwTransmissivityTop + lwAbsorptivityTop,
            sum3 = swReflectivityBottom + swTransmissivityBottom + swAbsorptivityBottom,
            sum4 = lwReflectivityBottom + lwTransmissivityBottom + lwAbsorptivityBottom;
    Q_ASSERT(eq(swReflectivityTop + swTransmissivityTop + swAbsorptivityTop, 1.));
    Q_ASSERT(eq(lwReflectivityTop + lwTransmissivityTop + lwAbsorptivityTop, 1.));
    Q_ASSERT(eq(swReflectivityBottom + swTransmissivityBottom + swAbsorptivityBottom, 1.));
    Q_ASSERT(eq(lwReflectivityBottom + lwTransmissivityBottom + lwAbsorptivityBottom, 1.));

//    // Correct the weighted sums
//    scaleToOne(swReflectivityTop,    swTransmissivityTop,    swAbsorptivityTop);
//    scaleToOne(lwReflectivityTop,    lwTransmissivityTop,    lwAbsorptivityTop);
//    scaleToOne(swReflectivityBottom, swTransmissivityBottom, swAbsorptivityBottom);
//    scaleToOne(lwReflectivityBottom, lwTransmissivityBottom, lwAbsorptivityBottom);
}

} //namespace


