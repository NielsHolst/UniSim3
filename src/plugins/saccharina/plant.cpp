/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "plant.h"

using namespace base;
using vector_op::sum;

namespace saccharina {

PUBLISH(Plant)

Plant::Plant(QString name, Box *parent)
    : Density(name, parent)
{
    help("keeps track of whole plant variables");
    Input(wSC).imports("../structure[C]");
    Input(wSN).imports("../structure[N]");
    Input(wCC).imports("../storeC[C]");
    Input(wNN).imports("../storeN[N]");
    Input(wPP).imports("../storeP[P]");
    Input(dws).imports("../PlantCompartment::*[dw]");
    Output(concC).unit("g C/g dw").help("Carbon concentration");
    Output(concN).unit("g N/g dw").help("Nitrogen concentration");
    Output(concP).unit("g P/g dw").help("Phosphorus concentration");
    Output(growthRate).unit("/d").help("Instantaneous biomass growth rate");
}

void Plant::initialize() {
    auto sz = dws.size();
    if (sz != 4)
      Exception("Expected 4 PlantCompartment boxes").value(sz).context(this);
}

void Plant::reset() {
  prevDw = 0;
  update();
}

void Plant::update() {
    dw    = sum(dws);
    C     = wSC + wCC;
    N     = wSN + wNN;
    P     = wPP;
    concC = (dw > 0.) ? C/dw : 0.;
    concN = (dw > 0.) ? N/dw : 0.;
    concP = (dw > 0.) ? P/dw : 0.;
    growthRate = (prevDw > 0) ? log(dw/prevDw) : 0.;
    prevDw = dw;
}

}
