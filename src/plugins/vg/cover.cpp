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
    : Layer(name, parent)
{
    help("holds cover radiation and heat parameters");
    useLayerAsInput();
    port("checkBoundaries")->equals(true);
}

} //namespace


