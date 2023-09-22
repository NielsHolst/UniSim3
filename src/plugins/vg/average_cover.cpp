/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "average_cover.h"
#include "face.h"

using namespace base;
using std::min;

namespace vg {

PUBLISH(AverageCover)

AverageCover::AverageCover(QString name, Box *parent)
    : AverageShelterLayer(name, parent)
{
    help("computes average cover radiation and heat parameters");
    Input(transmissivityReduction).imports("shelter[transmissivityReduction]");
    Input(swShading).imports("shelter/shading[swReflectivity]");
    Input(lwShading).imports("shelter/shading[lwReflectivity]");
}

void AverageCover::reset() {
    // Create six vectors, one or each face.
    // Each vector contains the UinsulationAdj values for the screens on that face
    for (int i = 0; i < 6; ++i)
            _Uinsulations[i].clear();

    int i = 0;
    for (Face *face : _faces) {
        // Insolation U values from screens installed on this face
        for (QString &screenMaterial : face->port("screenMaterials")->value<QVector<QString>>()) {
            if (screenMaterial != "none") {
                Box *screen = findOne<Box*>("shelter/products/screens/" + screenMaterial);
                _Uinsulations[i] <<  screen->port("Uinsulation")->valuePtr<double>();
            }
        }
        ++i;
    }
    update();
}


LayerParameters AverageCover::transform(const LayerParametersPtrs &p, const QVector<double> &adjustments) {
    LayerParameters adj;
    const double
        &transmissivityReduction(adjustments.at(0)),
        &swShading(adjustments.at(1)),
        &lwShading(adjustments.at(2));

    // Add shading to reflectivity
    adj.swReflectivityTop    = 1. - (1. - *p.swReflectivityTop)   *(1. - swShading);
    adj.lwReflectivityTop    = 1. - (1. - *p.lwReflectivityTop)   *(1. - lwShading);
    adj.swReflectivityBottom = 1. - (1. - *p.swReflectivityBottom)*(1. - swShading);
    adj.lwReflectivityBottom = 1. - (1. - *p.lwReflectivityBottom)*(1. - lwShading);

    // General transmissivity reduction is attributed to increased absorptivity, shared between top and bottom
    // But ensure that absorptivity + reflectivity <= 1.
    adj.swAbsorptivityTop    = min(adj.swAbsorptivityTop    + transmissivityReduction/2., 1. - adj.swReflectivityTop);
    adj.lwAbsorptivityTop    = min(adj.lwAbsorptivityTop    + transmissivityReduction/2., 1. - adj.lwReflectivityTop);
    adj.swAbsorptivityBottom = min(adj.swAbsorptivityBottom + transmissivityReduction/2., 1. - adj.swReflectivityBottom);
    adj.lwAbsorptivityBottom = min(adj.lwAbsorptivityBottom + transmissivityReduction/2., 1. - adj.lwReflectivityBottom);

    // Transmissivity takes rest
    adj.swTransmissivityTop     = 1. - adj.swReflectivityTop    - adj.swAbsorptivityTop;
    adj.lwTransmissivityTop     = 1. - adj.lwReflectivityTop    - adj.lwAbsorptivityTop;
    adj.swTransmissivityBottom  = 1. - adj.swReflectivityBottom - adj.swAbsorptivityBottom;
    adj.lwTransmissivityBottom  = 1. - adj.lwReflectivityBottom - adj.lwAbsorptivityBottom;

    // Unaffected by shading and transmissivity reduction
    adj.Utop         = *p.Utop;
    adj.Ubottom      = *p.Ubottom;
    adj.heatCapacity = *p.heatCapacity;
    return adj;
}


void AverageCover::update() {
    updateParameters(0, {transmissivityReduction, swShading, lwShading});
    correctUbottom();
}

void AverageCover::correctUbottom() {
    // Compute Ubottom weighed over all six surfaces
    double sumUface = 0;
    int i = 0;
    for (Face *face : _faces) {
        const LayerParametersPtrs &p(face->parameters(0));
        // Add up Uinsulation of screens and them to Ubottom of the cover
        // On one face, resistances are in a series
        double sumRface = 1./(*p.Ubottom);
        for (const double *Uinsulation : _Uinsulations[i])
            sumRface += 1./(*Uinsulation);
        // Overall, the resistances of the six surfaces are in parallel;
        // i.e. the conductancies (U=1/R) are additive, weighed by face area
        sumUface += areas.at(i)*1./sumRface;
        // Units: U [W/K/m2 layer]; sumUface [W/K]
        ++i;
    }
    Ubottom = sumUface/groundArea;
    // Unit: Ubottom [W/K/m2 ground]
}

} //namespace


