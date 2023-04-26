/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "par_budget.h"

#include <iostream>

using namespace base;
using namespace vector_op;

namespace vg {

PUBLISH(ParBudget)

ParBudget::ParBudget(QString name, Box *parent)
    : Box(name, parent)
{
    help("defines all model outputs");
    Input(sunPar).imports("sky[parFluxDown]");
    Input(growthLightsPar).imports("growthLights[parEmittedDown]");    
    Input(Pn).imports("crop/photosynthesis[Pn]");
    Input(cropCoverage).imports("crop[coverage]");

    Input(constructionReflection).imports("construction/geometry[reflection]");
    Input(faceAreas).imports("construction/shelter/*/area[value]");
    Input(faceReflections).imports("construction/shelter/*/cover[swReflectivityTop]");
    Input(faceLayer1Reflections).imports("construction/shelter/*/screens/layer1[swReflectivityTop]");
    Input(faceLayer2Reflections).imports("construction/shelter/*/screens/layer2[swReflectivityTop]");
    Input(faceLayer3Reflections).imports("construction/shelter/*/screens/layer3[swReflectivityTop]");
    Input(faceLayer1Areas).imports("construction/shelter/*/screens/layer1[faceArea]");
    Input(faceLayer2Areas).imports("construction/shelter/*/screens/layer2[faceArea]");
    Input(faceLayer3Areas).imports("construction/shelter/*/screens/layer3[faceArea]");

    Output(indoorsSunPar).help("Sun contribution to total PAR");
    Output(indoorsGrowthLightPar).help("Growth light contribution to total PAR");
    Output(indoorsTotalPar).imports("energyBudget[cropParFluxFromAbove]");
    Output(photosynthesis).unit("g dry mass/cultivated m2/h").help("Net crop growth rate");


}

void ParBudget::initialize() {
    _sensor = findMaybeOne<Box*>("sensor");
    auto n = faceAreas.size();
    if (n != 6)
        ThrowException("Missing greenhouse face. Six faces are needed").value(n).context(this);
    auto m = faceAreas.size();
    if (m != 6)
        ThrowException("Missing greenhouse cover. Six covers are needed").value(m).context(this);
}

void ParBudget::reset() {
    _totalArea = sum(faceAreas);
    update();
}

void ParBudget::update() {
    // Weighted averages: cover
    int n = 6;
    double wsum = 0.;
    for (int i=0; i<n; ++i)
        wsum += faceAreas.at(i)*faceReflections.at(i);
    double faceReflection = wsum/_totalArea;

    // Weighted averages: layer1
    n = faceLayer1Areas.size();
    wsum = 0.;
    for (int i=0; i<n; ++i)
        wsum += faceLayer1Areas.at(i)*faceLayer1Reflections.at(i);
    double layer1Reflection = wsum/_totalArea;

    // Weighted averages: layer2
    n = faceLayer2Areas.size();
    wsum = 0.;
    for (int i=0; i<n; ++i)
        wsum += faceLayer2Areas.at(i)*faceLayer2Reflections.at(i);
    double layer2Reflection = wsum/_totalArea;

    // Weighted averages: layer3
    n = faceLayer3Areas.size();
    wsum = 0.;
    for (int i=0; i<n; ++i)
        wsum += faceLayer3Areas.at(i)*faceLayer3Reflections.at(i);
    double layer3Reflection = wsum/_totalArea;

    // Net reflection
    double totalReflection =
        (1. - constructionReflection)*(1. - faceReflection)*
        (1. - layer1Reflection)*(1. - layer2Reflection)*(1. - layer3Reflection);

    // PAR results
    indoorsSunPar = (1. - totalReflection)*sunPar;
    indoorsGrowthLightPar = growthLightsPar;
    indoorsTotalPar = indoorsSunPar + indoorsGrowthLightPar;

    // HACK
    if (_sensor) {
        indoorsTotalPar = _sensor->port("indoorsLightIntensity")->value<double>();
        indoorsSunPar = std::max(indoorsTotalPar - indoorsGrowthLightPar, 0.);
        std::cout << "ParBudget::update() " << indoorsTotalPar;
    }

    // Crop growth rate scaled to cultivated area
    photosynthesis = Pn/cropCoverage;
}


} //namespace

