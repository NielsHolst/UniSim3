/* Copyright 2019-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Teis Boderskov, Aarhus University [tebo atbios.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "demand_structure.h"

using namespace base;

namespace saccharina {

PUBLISH(DemandStructure)

DemandStructure::DemandStructure(QString name, Box *parent)
    : Density(name, parent)
{
    help("computes demand for structural growth");
    Input(g).unit("g dw/g dw/d").help("Optimal growth rate");
    Input(concC).unit("g C/g dw").help("Carbon concentration");
    Input(concN).unit("g N/g dw").help("Nitrogen concentration");
    Input(phiT).equals(1.).unit("[0;1]").help("Scaling factor for temperature");
    Input(phiSal).equals(1.).unit("[0;1]").help("Scaling factor for salinity");
    Input(biomass).unit("g dw/m").help("Whole-plant biomass");
}

void DemandStructure::reset() {
   update();
}

void DemandStructure::update() {
    dw = phiT*phiSal*g*biomass;
    C = concC*dw;
    N = concN*dw;
}

}
