/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "demand_exudation.h"

using namespace base;
using vector_op::sum;

namespace saccharina {

PUBLISH(DemandExudation)

DemandExudation::DemandExudation(QString name, Box *parent)
    : Density(name, parent)
{
    help("computes exudation");
    Input(biomass).unit("g dw/m").help("Exuding biomass");
    Input(eC).unit("g C/g dw/d").help("Carbon exudation rate");
    Input(eN).unit("g N/g dw/d").help("Nitrogen exudation rate");
    Input(eP).unit("g P/g dw/d").help("Phosphorus exudation rate");
}

void DemandExudation::reset() {
   update();
}

void DemandExudation::update() {
    C = eC*biomass;
    N = eN*biomass;
    P = eP*biomass;
}

}
