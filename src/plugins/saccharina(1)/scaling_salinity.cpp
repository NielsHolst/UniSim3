/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "scaling_salinity.h"

using namespace base;

namespace saccharina {

PUBLISH(ScalingSalinity)

ScalingSalinity::ScalingSalinity(QString name, Box *parent)
    : Box(name, parent)
{
    help("scales growth rate by salinity");
    Input(S).imports("env[S]").unit("PSU").help("Environmental salinity level");
    Input(S50).equals(16.).unit("PSU").help("Salinity that is 50% optimal");
    Input(Sopt).equals(25.).unit("PSU").help("Minimum salinity that is optimal");
    Output(value).unit("[0;1]").help("Scaling factor");
}

void ScalingSalinity::reset() {
   update();
}

void ScalingSalinity::update() {
    if (S < S50)
        value = S/S50/2.;
    else if (S < Sopt)
        value = 0.5 + (S-S50)/(Sopt-S50)/2.;
    else
        value = 1.;
}

}

