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
    help("scales photosynthesis by plant nitrogen concentration");
    Input(concN).unit("g N/g dw").help("Plant nitrogen concentration");
    Input(concNmin).unit("g N/g dw").help("Minimum plant nitrogen concentration for growth");
    Input(concNopt).unit("g N/g dw").help("Optimal plant nitrogen concentration for growth");
    Output(value).unit("[0;1]").help("Scaling factor");
}

void ScalingNitrogen::reset() {
   update();
}

void ScalingNitrogen::update() {
    if (concN < concNmin)
        value = 0.;
    else if (concN < concNopt)
        value = (concN - concNmin)/(concNopt - concNmin);
    else
        value = 1.;
}

}

