/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/vector_op.h>
#include "average_shelter_layer.h"
#include "face.h"

using namespace base;

namespace vg {

AverageShelterLayer::AverageShelterLayer(QString name, Box *parent)
    : Layer(name, parent)
{
    help("computes average parameters for covers and screens");
    Input(areas).imports("shelter/faces/*[area]");
    Input(weights).imports("shelter/faces/*[weight]");
    Input(groundArea).imports("gh/geometry[groundArea]");
    useLayerAsOutput();
    port("Utop")->unit("W/K/m2 ground");
    port("Ubottom")->unit("W/K/m2 ground");
    port("heatCapacity")->unit("J/K/m2 ground");
}

void AverageShelterLayer::initialize() {
    _faces = findMany<Face*>("shelter/faces/*");
    if (_faces.size() != 6)
        ThrowException("Expected six faces").value("\n" + fullNames(_faces).join("\n")).context(this);

    Q_ASSERT(weights.size() == 6);
    Q_ASSERT(areas.size() == 6);
    double sum2 = vector_op::sumOfProducts(areas, weights);
    for (int i=0; i<6; ++i) {
        _weights1[i] = areas.at(i)/groundArea;
        _weights2[i] = weights.at(i)*areas.at(i)/sum2;
    }
}

namespace {
    void scaleToOne(double &a, double &b, double &c) {
        const double sum = a + b + c;
        a /= sum;
        b /= sum;
        c /= sum;
    }
}

#define WEIGHT1(x) x += _weights1[i]*p.x // Converts from [W/K/m2 layer] to [W/K/m2 ground]
#define WEIGHT2(x) x += _weights2[i]*p.x

void AverageShelterLayer::updateParameters(const int layerIndex, const QVector<double> &adjustments) {
    // Compute Layer parameter values weighed over all six surfaces
    swReflectivityTop = swReflectivityBottom = swTransmissivityTop = swTransmissivityBottom = swAbsorptivityTop = swAbsorptivityBottom =
    lwReflectivityTop = lwReflectivityBottom = lwTransmissivityTop = lwTransmissivityBottom = lwAbsorptivityTop = lwAbsorptivityBottom =
    heatCapacity = Utop = Ubottom = 0.;

    int i = 0;
    for (Face *face : _faces) {
        LayerParameters p = transform(face->parameters(layerIndex), adjustments);
        WEIGHT2(swReflectivityTop);
        WEIGHT2(swReflectivityBottom);
        WEIGHT2(swTransmissivityTop);
        WEIGHT2(swTransmissivityBottom);
        WEIGHT2(lwReflectivityTop);
        WEIGHT2(lwReflectivityBottom);
        WEIGHT2(lwTransmissivityTop);
        WEIGHT2(lwTransmissivityBottom);
        WEIGHT2(swAbsorptivityTop);
        WEIGHT2(swAbsorptivityBottom);
        WEIGHT2(lwAbsorptivityTop);
        WEIGHT2(lwAbsorptivityBottom);
        // heatCapacity, UTop and UBottom are converted to [W/K/m2 ground]
        WEIGHT1(heatCapacity);
        WEIGHT1(Utop);
        WEIGHT1(Ubottom);
        ++i;
    }
    // Correct the weighted sums
    scaleToOne(swReflectivityTop,    swTransmissivityTop,    swAbsorptivityTop);
    scaleToOne(lwReflectivityTop,    lwTransmissivityTop,    lwAbsorptivityTop);
    scaleToOne(swReflectivityBottom, swTransmissivityBottom, swAbsorptivityBottom);
    scaleToOne(lwReflectivityBottom, lwTransmissivityBottom, lwAbsorptivityBottom);
}

} //namespace


