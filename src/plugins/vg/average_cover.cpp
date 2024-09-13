/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/box_builder.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "average_cover.h"
#include "face.h"

using namespace base;
using std::min;
using phys_math::minmax;

namespace vg {

PUBLISH(AverageCover)

AverageCover::AverageCover(QString name, Box *parent)
    : AverageShelterLayer(name, parent)
{
    help("computes average cover radiation and heat parameters");
    Input(transmissivityReduction).imports("shelter[transmissivityReduction]");
    Input(swShading).computes("if exists(shelter/shading[swReflectivity]) then shelter/shading[swReflectivity] else 0.");
    Input(lwShading).computes("if exists(shelter/shading[lwReflectivity]) then shelter/shading[lwReflectivity] else 0.");
    Input(faceAreas).imports("shelter/faces/*[area]");
    Input(screenStates).imports("../screens/*[state]");
}

void AverageCover::reset() {
    // Create six vectors, one for each face.
    // Each vector contains the Uinsulation values for the screens on that face
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

    // Reduce transmissivity
    adj.swTransmissivityTop     = *p.swTransmissivityTop    * (1.- transmissivityReduction) * (1. - swShading);
    adj.lwTransmissivityTop     = *p.lwTransmissivityTop    * (1.- transmissivityReduction) * (1. - lwShading);
    adj.swTransmissivityBottom  = *p.swTransmissivityBottom * (1.- transmissivityReduction) * (1. - swShading);
    adj.lwTransmissivityBottom  = *p.lwTransmissivityBottom * (1.- transmissivityReduction) * (1. - lwShading);

    double
        sumRS = transmissivityReduction + swShading,
        partR = (sumRS > 0.) ? transmissivityReduction/sumRS : 0.,
//      partS = (sumRS > 0.) ? swShading/sumRS : 0.,
        swAbsTopChange = partR*(*p.swTransmissivityTop    - adj.swTransmissivityTop   ),
        lwAbsTopChange = partR*(*p.lwTransmissivityTop    - adj.lwTransmissivityTop   ),
        swAbsBotChange = partR*(*p.swTransmissivityBottom - adj.swTransmissivityBottom),
        lwAbsBotChange = partR*(*p.lwTransmissivityBottom - adj.lwTransmissivityBottom);

    // Increase absorptivity by change in transmissivity
    adj.swAbsorptivityTop    = std::min(*p.swAbsorptivityTop    + swAbsTopChange, 1. - *p.swReflectivityTop);
    adj.lwAbsorptivityTop    = std::min(*p.lwAbsorptivityTop    + lwAbsTopChange, 1. - *p.lwReflectivityTop);
    adj.swAbsorptivityBottom = std::min(*p.swAbsorptivityBottom + swAbsBotChange, 1. - *p.swReflectivityBottom);
    adj.lwAbsorptivityBottom = std::min(*p.lwAbsorptivityBottom + lwAbsBotChange, 1. - *p.lwReflectivityBottom);

    // Take rest
    adj.swReflectivityTop    = 1. - adj.swTransmissivityTop    - adj.swAbsorptivityTop;
    adj.lwReflectivityTop    = 1. - adj.lwTransmissivityTop    - adj.lwAbsorptivityTop;
    adj.swReflectivityBottom = 1. - adj.swTransmissivityBottom - adj.swAbsorptivityBottom;
    adj.lwReflectivityBottom = 1. - adj.lwTransmissivityBottom - adj.lwAbsorptivityBottom;

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
    // U is a conductance
    // R = 1/U is a resistance
    // Compute Ubottom weighed over all six surfaces
    // This overrides the calculation of Ubottom in updateParameters(...)
    Ubottom = 0;
    int i = 0;
    for (Face *face : _faces) {
        const LayerParametersPtrs &p(face->parameters(0));
        // Add up Uinsulation of screens and them to Ubottom of the cover
        // On one face, resistances are in a series and hence additive
        double sumRface = 1./(*p.Ubottom);
        int layer = 0;
        for (const double *Uinsulation : _Uinsulations[i])
            sumRface += screenStates.at(layer++)/(*Uinsulation);
        // Overall, the resistances of the six surfaces are in parallel;
        // i.e. the conductancies (U=1/R) are additive
        Ubottom += faceAreas.at(i++)/sumRface;
    }
    Ubottom /= groundArea;
}

} //namespace


