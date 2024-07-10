/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "screen.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(Screen)

Screen::Screen(QString name, Box *parent)
    : LayerAsInput(name, parent)
{
    help("holds screen radiation and heat parameters");
    Input(energySaving).unit("%").help("Documented energy saving used for calibration");
    Input(Uinsulation).equals(infinity()).unit("W/K/m2 layer").help("Insulation effect");
    Input(UinsulationEffectivity).equals(1.).unit("[0;1]").help("Effectivity of Uinsulation");
    port("checkBoundaries")->equals(true);
}

} //namespace

