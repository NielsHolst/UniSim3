/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "screen.h"

using namespace base;
using namespace phys_math;
using namespace TestNum;

namespace vg {

PUBLISH(Screen)

Screen::Screen(QString name, Box *parent)
    : Box(name, parent),
      Layer(name, parent),
      LayerAdjusted(name, parent)
{
    help("models a screen layer as influenced by its state");
    Input(state).help("Proportion drawn (0=fully withdrawn; 1=fully drawn").unit("[0;1]");
    Input(coverPerGroundArea).imports("geometry[coverPerGroundArea]");
}

void Screen::reset() {
    state = 0.;
    update();
}

void Screen::update() {
    snapTo(state, 0., 1e-3);
    snapTo(state, 1., 1e-3);
    if ((state < 0.) || (state > 1.))
        ThrowException("Screen state out of [0;1] bounds").value(state).context(this);

    swReflectivityTopAdj    = state*swReflectivityTop;
    swReflectivityBottomAdj = state*swReflectivityBottom;
    swAbsorptivityTopAdj    = state*(1. - swReflectivityTop    - swTransmissivityTop);
    swAbsorptivityBottomAdj = state*(1. - swReflectivityBottom - swTransmissivityBottom);

    lwReflectivityTopAdj    = state*lwReflectivityTop;
    lwReflectivityBottomAdj = state*lwReflectivityBottom;
    lwAbsorptivityTopAdj    = state*(1. - lwReflectivityTop    - lwTransmissivityTop);
    lwAbsorptivityBottomAdj = state*(1. - lwReflectivityBottom - lwTransmissivityBottom);

    swTransmissivityTopAdj     = 1. - swReflectivityTopAdj    - swAbsorptivityTopAdj;
    swTransmissivityBottomAdj  = 1. - swReflectivityBottomAdj - swAbsorptivityBottomAdj;

    lwTransmissivityTopAdj     = 1. - lwReflectivityTopAdj    - lwAbsorptivityTopAdj;
    lwTransmissivityBottomAdj  = 1. - lwReflectivityBottomAdj - lwAbsorptivityBottomAdj;

    double swTop = swReflectivityTopAdj + swAbsorptivityTopAdj + swTransmissivityTopAdj,
           lwTop = lwReflectivityTopAdj + lwAbsorptivityTopAdj + lwTransmissivityTopAdj,
           swBottom = swReflectivityBottomAdj + swAbsorptivityBottomAdj + swTransmissivityBottomAdj,
           lwBottom = lwReflectivityBottomAdj + lwAbsorptivityBottomAdj + lwTransmissivityBottomAdj;
    if (ne(swTop, 1.))
        ThrowException("Radiation parameters must sum to 1").value(swTop).hint("swTop").context(this);
    if (ne(lwTop, 1.))
        ThrowException("Radiation parameters must sum to 1").value(lwTop).hint("lwTop").context(this);
    if (ne(swBottom, 1.))
        ThrowException("Radiation parameters must sum to 1").value(swBottom).hint("swTop").context(this);
    if (ne(lwBottom, 1.))
        ThrowException("Radiation parameters must sum to 1").value(lwBottom).hint("lwTop").context(this);

    UtopAdj         = state*coverPerGroundArea*Utop;
    UbottomAdj      = state*coverPerGroundArea*Ubottom;
    heatCapacityAdj = state*coverPerGroundArea*heatCapacity;
}

} //namespace

