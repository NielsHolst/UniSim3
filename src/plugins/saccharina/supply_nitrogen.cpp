/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "supply_nitrogen.h"

using namespace base;
using std::min;

namespace saccharina {

PUBLISH(SupplyNitrogen)

SupplyNitrogen::SupplyNitrogen(QString name, Box *parent)
    : SupplyBase(name, parent)
{
    help("computes uptake supply of nitrogen");
    Input(beta).unit("g N/g dw/molar N").help("Nitrogen uptake rate");
    Input(area).unit("m2/m").help("Leaf area");
    Input(waterN).unit("molar N").help("Water nitrogen concentration");
}

void SupplyNitrogen::updateValue() {
    value = (demand > 0.) ? demand*(1. - exp(-beta*area*waterN/demand)) : 0.;
}

}
