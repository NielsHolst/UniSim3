/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "supply_carbon.h"

using namespace base;
using std::min;

namespace saccharina {

PUBLISH(SupplyCarbon)

SupplyCarbon::SupplyCarbon(QString name, Box *parent)
    : SupplyBase(name, parent)
{
    help("computes photosynthetic supply of carbon");
    Input(alpha).unit("g C/g dw/(mol PAR/m2)").help("Photosynthetic efficiency");
    Input(k).help("Canopy light extinction coefficient");
    Input(lai).unit("m2/m2").help("Leaf area index");
    Input(czArea).unit("m2/m").help("Crown zone area");
    Input(phiN).unit("[0;1]").help("Scaling for plant nitrogen concentration");
    Input(Id).unit("mol PAR/m2/d").help("Solar PAR at plant canopy depth");
    Output(Iabsorbed).unit("mol PAR/m2/d").help("Solar PAR absorbed");
    Output(propIabsorbed).unit("[0;1]").help("Proportion of solar PAR absorbed");
}

void SupplyCarbon::updateValue() {
    Iabsorbed = czArea*Id*(1 - exp(-k*lai));
    value = demand*(1. - exp(-alpha*phiN*Iabsorbed/demand));
    propIabsorbed = (Id > 0.) ? Iabsorbed/Id : 0.;
}

}
