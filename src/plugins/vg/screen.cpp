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
    Input(Uinsulation).unit("W/K/m2 layer").equals(1e16).help("Insulation effect on cover");
    Output(UinsulationAdj).unit("W/K/m2 layer").help("Insulation effect on cover adjusted for state");
}

void Screen::reset() {
    checkInputs();
    state = 0.;
    update();
}

void Screen::update() {
    snapTo(state, 0., 1e-3);
    snapTo(state, 1., 1e-3);
    if ((state < 0.) || (state > 1.))
        ThrowException("Screen state out of [0;1] bounds").value(state).context(this);

    swAbsorptivityTopAdj      = state*swAbsorptivityTop;
    swReflectivityTopAdj      = state*swReflectivityTop;
    swTransmissivityTopAdj    = 1. - swAbsorptivityTopAdj - swReflectivityTopAdj;

    swAbsorptivityBottomAdj   = state*swAbsorptivityBottom;
    swReflectivityBottomAdj   = state*swReflectivityBottom;
    swTransmissivityBottomAdj = 1. - swAbsorptivityBottomAdj - swReflectivityBottomAdj;

    lwAbsorptivityTopAdj      = state*lwAbsorptivityTop;
    lwReflectivityTopAdj      = state*lwReflectivityTop;
    lwTransmissivityTopAdj    = 1. - lwAbsorptivityTopAdj - lwReflectivityTopAdj;

    lwAbsorptivityBottomAdj   = state*lwAbsorptivityBottom;
    lwReflectivityBottomAdj   = state*lwReflectivityBottom;
    lwTransmissivityBottomAdj = 1. - lwAbsorptivityBottomAdj - lwReflectivityBottomAdj;

    UtopAdj         = Utop;
    UbottomAdj      = Ubottom;
    UinsulationAdj  = Uinsulation/state;
    heatCapacityAdj = heatCapacity;

    checkOutputs();
}

} //namespace

