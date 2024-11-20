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
    Input(propFrame).imports("shelter[propFrame]");
    Input(swReflectivityFrame).imports("shelter[swReflectivityFrame]");
    Input(lwReflectivityFrame).imports("shelter[lwReflectivityFrame]");
    Input(heatCapacityFrame).imports("shelter[heatCapacityFrame]");
    Input(swTransmissivityChalk).computes("if exists(shelter/shading[swTransmissivity]) then shelter/shading[swTransmissivity] else 1.");
    Input(lwTransmissivityChalk).computes("if exists(shelter/shading[lwTransmissivity]) then shelter/shading[lwTransmissivity] else 1.");
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


//LayerParameters AverageCover::transform(const LayerParametersPtrs &p, const QVector<double> &adjustments) {
LayerParameters AverageCover::transform(const LayerParameters *p, const QVector<double> &adjustments) {
    LayerParameters adj;
    const double
        &propFrame(adjustments.at(0)),
        &swReflectivityFrame(adjustments.at(1)),
        &lwReflectivityFrame(adjustments.at(2)),
        &swTransmissivityChalk(adjustments.at(3)),
        &lwTransmissivityChalk(adjustments.at(4)),
        &heatCapacityFrame(adjustments.at(5));

    // Reduce transmissivity by frame and chalk
    adj.swTransmissivityTop     = p->swTransmissivityTop    * (1. - propFrame) * swTransmissivityChalk;
    adj.lwTransmissivityTop     = p->lwTransmissivityTop    * (1. - propFrame) * lwTransmissivityChalk;
    adj.swTransmissivityBottom  = p->swTransmissivityBottom * (1. - propFrame) * swTransmissivityChalk;
    adj.lwTransmissivityBottom  = p->lwTransmissivityBottom * (1. - propFrame) * lwTransmissivityChalk;

    // Weigh absorptivities across frame and sheet
    adj.swAbsorptivityTop    = p->swAbsorptivityTop    * (1. - propFrame) + (1. - swReflectivityFrame) * propFrame ;
    adj.lwAbsorptivityTop    = p->lwAbsorptivityTop    * (1. - propFrame) + (1. - lwReflectivityFrame) * propFrame ;
    adj.swAbsorptivityBottom = p->swAbsorptivityBottom * (1. - propFrame) + (1. - swReflectivityFrame) * propFrame ;
    adj.lwAbsorptivityBottom = p->lwAbsorptivityBottom * (1. - propFrame) + (1. - lwReflectivityFrame) * propFrame ;

    // Reflectivity takes rest
    adj.swReflectivityTop    = 1. - adj.swTransmissivityTop    - adj.swAbsorptivityTop;
    adj.lwReflectivityTop    = 1. - adj.lwTransmissivityTop    - adj.lwAbsorptivityTop;
    adj.swReflectivityBottom = 1. - adj.swTransmissivityBottom - adj.swAbsorptivityBottom;
    adj.lwReflectivityBottom = 1. - adj.lwTransmissivityBottom - adj.lwAbsorptivityBottom;

    // Weigh heat capacities across frame and sheet
    adj.heatCapacity = p->heatCapacity*(1. - propFrame) + heatCapacityFrame*propFrame;

    // Unaffected by frame and chalk
    adj.Utop         = p->Utop;
    adj.Ubottom      = p->Ubottom;
    return adj;
}


void AverageCover::update() {
    updateParameters(0, {propFrame, swReflectivityFrame, lwReflectivityFrame, swTransmissivityChalk, lwTransmissivityChalk, heatCapacityFrame});
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
        const LayerParameters &p(*face->parameters(0));
        // Add up Uinsulation of screens and them to Ubottom of the cover
        // On one face, resistances are in a series and hence additive
        double sumRface = 1./(p.Ubottom);
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


