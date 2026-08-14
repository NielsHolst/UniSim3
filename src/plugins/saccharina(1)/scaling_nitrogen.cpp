/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "scaling_nitrogen.h"

using namespace base;

namespace saccharina {

PUBLISH(ScalingNitrogen)

ScalingNitrogen::ScalingNitrogen(QString name, Box *parent)
    : Box(name, parent)
{
    help("scales photosynthesis by plant nitrogen");
    Input(minValue).equals(0.5).unit("[0;1]").help("Minimum scaling value");
    Input(concN).imports("plant[concN]");
    Input(optNConc).imports("plant[optNConc]");
    Output(value).unit("[0;1]").help("Scaling factor");
}

void ScalingNitrogen::reset() {
   update();
}

void ScalingNitrogen::update() {
    value = minValue + (1. - minValue)*concN/optNConc;
}

}

