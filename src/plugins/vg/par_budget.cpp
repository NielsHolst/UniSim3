/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "par_budget.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(ParBudget)

ParBudget::ParBudget(QString name, QObject *parent)
    : Box(name, parent)
{
    help("defines all model outputs");
    Input(sunPar).imports("sky[parFluxDown]");
    Input(growthLightsPar).imports("growthLights[parFluxDown]");
    Input(indoorsTotalPar).imports("energyBudget[cropParFluxFromAbove]");
    Input(Pn).imports("crop/photosynthesis[Pn]");
    Input(cropCoverage).imports("crop[coverage]");
    Output(indoorsSunPar).help("Sun contribution to total PAR");
    Output(indoorsGrowthLightPar).help("Growth light contribution to total PAR");
    Output(photosynthesis).unit("g dry mass/cultivated m2/h").help("Net crop growth rate");
}

void ParBudget::reset() {
    update();
}

void ParBudget::update() {
    // Total par hitting the crop (ideally)
    double cropTotalPar = sunPar + growthLightsPar/cropCoverage;
    if (cropTotalPar > 1e-6) {
        // Sun proportion of ideal total
        double propSun = sunPar/cropTotalPar;
        indoorsSunPar         = propSun     *indoorsTotalPar;
        indoorsGrowthLightPar = (1.-propSun)*indoorsTotalPar;
    }
    else {
        indoorsSunPar = indoorsGrowthLightPar = 0.;
    }
    // Crop growth rate scaled to cultivated area
    photosynthesis = Pn/cropCoverage;
}


} //namespace

