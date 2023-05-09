/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "floor.h"

using namespace base;

namespace vg {

PUBLISH(Floor)

Floor::Floor(QString name, Box *parent)
    : Box(name, parent),
      Layer(name, parent),
      LayerAdjusted(name, parent)
{
    help("holds floor radiation and heat parameters");
}

void Floor::reset() {
    checkInputs();

    swReflectivityTopAdj      = swReflectivityTop   ;
    lwReflectivityTopAdj      = lwReflectivityTop   ;
    swReflectivityBottomAdj   = swReflectivityBottom;
    lwReflectivityBottomAdj   = lwReflectivityBottom;

    swTransmissivityTopAdj    = swTransmissivityTop   ;
    lwTransmissivityTopAdj    = lwTransmissivityTop   ;
    swTransmissivityBottomAdj = swTransmissivityBottom;
    lwTransmissivityBottomAdj = lwTransmissivityBottom;

    swAbsorptivityTopAdj      = swAbsorptivityTop   ;
    lwAbsorptivityTopAdj      = lwAbsorptivityTop   ;
    swAbsorptivityBottomAdj   = swAbsorptivityBottom;
    lwAbsorptivityBottomAdj   = lwAbsorptivityBottom;

    UtopAdj         = Utop;
    UbottomAdj      = Ubottom;
    heatCapacityAdj = heatCapacity;

    checkOutputs();
}

void Floor::update() {
}

} //namespace


