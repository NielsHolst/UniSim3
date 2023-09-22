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
    : Layer(name, parent)
{
    help("holds screen radiation and heat parameters");
    useLayerAsInput();
    Input(energySaving).unit("%").help("Documented energy saving used for calibration");
    Input(Uinsulation).equals(infinity()).unit("W/K/m2 layer").help("Insulation effect");
    Input(UinsulationEffectivity).equals(1.).unit("[0;1]").help("Effectivity of Uinsulation");
}

} //namespace

