/* Copyright 2019-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Teis Boderskov, Aarhus University [tebo atbios.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "scaling_phosphorus.h"

using namespace base;

namespace saccharina {

PUBLISH(ScalingPhosphorus)

ScalingPhosphorus::ScalingPhosphorus(QString name, Box *parent)
    : Box(name, parent)
{
    help("scales photosynthesis by plant phosphorus concentration");
    Input(concP).unit("g P/g dw").help("Plant phosphorus concentration");
    Input(concPopt).unit("g P/g dw").help("Optimal plant phosphorus concentration for growth");
    Output(value).unit("[0;1]").help("Scaling factor");
}

void ScalingPhosphorus::reset() {
   update();
}

void ScalingPhosphorus::update() {
    value = (concP < concPopt) ? concP/concPopt : 1.;
}

}

