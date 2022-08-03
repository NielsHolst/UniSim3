/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "area_erosion.h"

using namespace base;

namespace saccharina {

PUBLISH(AreaErosion)

AreaErosion::AreaErosion(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates area loss by erosion");
    Input(A).imports("area[value]");
    Input(e50).equals(1.4).unit("dm2").help("Area at which erosion reaches 50%");
    Input(eSlope).equals(-10.).help("Slope at A50");
    Input(eMax).equals(0.0003).unit("h-1").help("Max. erosion rate");
    Input(timeStepSecs).imports("calendar[timeStepSecs]");
    Output(value).unit("dm2").help("Area lost");
}

void AreaErosion::reset() {
   update();
}

void AreaErosion::update() {
    double
        rate = (A == 0.) ? 0. : eMax/(1. + exp(eSlope*(log(A) - log(e50)))),
        dt = timeStepSecs/3600.;
    value = A*rate*dt;
}

}
