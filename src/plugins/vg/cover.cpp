/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "cover.h"

using namespace base;

namespace vg {

PUBLISH(Cover)

Cover::Cover(QString name, Box *parent)
    : LayerAsInput(name, parent)
{
    help("holds cover radiation and heat parameters");
    port("checkBoundaries")->equals(true);
}

} //namespace


