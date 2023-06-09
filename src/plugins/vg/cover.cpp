/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "cover.h"

using namespace base;

namespace vg {

PUBLISH(Cover)

Cover::Cover(QString name, Box *parent)
    : Box(name, parent),
      Layer(name, parent),
      LayerAdjusted(name, parent)
{
    help("holds cover radiation and heat parameters");
    Input(transmissivityReduction).unit("[0;1]").help("Reduced cover transmission due to beams, dirt, etc.");
    Input(swShading).unit("[0;1]").help("Additional short-wave reflectivity caused by chalk");
    Input(lwShading).unit("[0;1]").help("Additional long-wave reflectivity caused by chalk");
}

void Cover::reset() {
    checkInputs();
    update();
}

void Cover::update() {
    // Add shading to reflectivity
    swReflectivityTopAdj    = 1. - (1. - swReflectivityTop)*(1. - swShading);
    lwReflectivityTopAdj    = 1. - (1. - lwReflectivityTop)*(1. - lwShading);
    swReflectivityBottomAdj = 1. - (1. - swReflectivityBottom)*(1. - swShading);
    lwReflectivityBottomAdj = 1. - (1. - lwReflectivityBottom)*(1. - lwShading);

    // Absorptivity is unaffacted but hold upper limit
    swAbsorptivityTopAdj     = (swAbsorptivityTop + swReflectivityTopAdj < 1.) ?
                                swAbsorptivityTop : 1. - swReflectivityTopAdj;

    lwAbsorptivityTopAdj     = (lwAbsorptivityTop + lwReflectivityTopAdj < 1.) ?
                                lwAbsorptivityTop : 1. - lwReflectivityTopAdj;

    swAbsorptivityBottomAdj  = (swAbsorptivityBottom + swReflectivityBottomAdj < 1.) ?
                                swAbsorptivityBottom : 1. - swReflectivityBottomAdj;

    lwAbsorptivityBottomAdj  = (lwAbsorptivityBottom + lwReflectivityBottomAdj < 1.) ?
                                lwAbsorptivityBottom : 1. - lwReflectivityBottomAdj;

    // Transmissivity takes rest
    swTransmissivityTopAdj     = 1. - swReflectivityTopAdj    - swAbsorptivityTopAdj;
    lwTransmissivityTopAdj     = 1. - lwReflectivityTopAdj    - lwAbsorptivityTopAdj;
    swTransmissivityBottomAdj  = 1. - swReflectivityBottomAdj - swAbsorptivityBottomAdj;
    lwTransmissivityBottomAdj  = 1. - lwReflectivityBottomAdj - lwAbsorptivityBottomAdj;

    // Unaffected by chalk
    UtopAdj         = Utop;
    UbottomAdj      = Ubottom;
    heatCapacityAdj = heatCapacity;
    checkOutputs();
}

} //namespace


