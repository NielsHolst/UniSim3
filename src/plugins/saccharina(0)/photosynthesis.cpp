/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "photosynthesis.h"

using namespace base;
using phys_math::PI;

namespace saccharina {

PUBLISH(Photosynthesis)

Photosynthesis::Photosynthesis(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the rate of photosynthesis");
    Input(Iabsorbed).imports("../lightAbsorption[value]");
    Input(area).imports("area[value]");
    Input(frondAngle).imports("area[frondAngle]");
    Input(demand).imports("demand/carbonTotal[value]");
    Input(alpha).unit("g C/mol PAR").help("Photosynthetic efficiency ");
    Input(fTemp).equals(1).unit("[0;1]").help("Temperature scaling on alpha");
    Input(fNitrogen).equals(1).unit("[0;1]").help("Nitrogen scaling on alpha");
    Input(timeStepSecs).imports("calendar[timeStepSecs]");
    Input(Pmax).equals(0.000444).help("Maximum hotosynthetic rate").unit("g C/dm2/h");
    Output(supply).unit("g C").help("Photosynthetic supply");
    Output(sdRatio).unit("[0;1]").help("Supply/Demand ratio");
    Output(P).unit("g C/dm2/h").help("Photosynthetic rate");
    Output(Punlimited).unit("g C/dm2/h").help("Photosynthetic rate, if unlimited by Pmax");
}


void Photosynthesis::reset() {
   update();
}

void Photosynthesis::update() {
    const double c = 3600e-6;
    supply = (demand<1e-16) ?
                0. :
                demand*(1. - exp(-alpha*Iabsorbed*fTemp*fNitrogen*c/demand));

    double dt = timeStepSecs/3600.;
    P = Punlimited = supply/area/dt;
    if (P>Pmax) {
        P = Pmax;
        supply = P*area*dt;
    }

    sdRatio = (demand==0.) ? 0. : supply/demand;
}

}
