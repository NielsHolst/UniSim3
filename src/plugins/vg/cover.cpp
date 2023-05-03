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
    Input(swReflectivityChalk).unit("[0;1]").help("Additional short-wave reflectivity caused by chalk");
    Input(lwReflectivityChalk).unit("[0;1]").help("Additional long-wave reflectivity caused by chalk");
}

void Cover::reset() {
    checkInputs();
    update();
}

void Cover::update() {
    // Add chalk to reflectivity
    swReflectivityTopAdj    = std::min(swReflectivityTop    + swReflectivityChalk, 1.);
    lwReflectivityTopAdj    = std::min(lwReflectivityTop    + lwReflectivityChalk, 1.);
    swReflectivityBottomAdj = std::min(swReflectivityBottom + swReflectivityChalk, 1.);
    lwReflectivityBottomAdj = std::min(lwReflectivityBottom + lwReflectivityChalk, 1.);

    // Subtract chalk from transmissivity
    swTransmissivityTopAdj     = std::max(swTransmissivityTop     - swReflectivityChalk, 0.);
    lwTransmissivityTopAdj     = std::max(lwTransmissivityTop     - lwReflectivityChalk, 0.);
    swTransmissivityBottomAdj  = std::max(swTransmissivityBottom  - swReflectivityChalk, 0.);
    lwTransmissivityBottomAdj  = std::max(lwTransmissivityBottom  - lwReflectivityChalk, 0.);

    // Absorptivity takes the rest
    swAbsorptivityTopAdj     = 1. - swReflectivityTopAdj    - swTransmissivityTopAdj;
    lwAbsorptivityTopAdj     = 1. - lwReflectivityTopAdj    - lwTransmissivityTopAdj;
    swAbsorptivityBottomAdj  = 1. - swReflectivityBottomAdj - swTransmissivityBottomAdj;
    lwAbsorptivityBottomAdj  = 1. - lwReflectivityBottomAdj - lwTransmissivityBottomAdj;

    // Unaffected by chalk
    UtopAdj         = Utop;
    UbottomAdj      = Ubottom;
    heatCapacityAdj = heatCapacity;
    checkOutputs();
}

} //namespace


