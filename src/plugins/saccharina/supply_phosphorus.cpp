/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "supply_phosphorus.h"

using namespace base;
using std::min;

namespace saccharina {

PUBLISH(SupplyPhosphorus)

SupplyPhosphorus::SupplyPhosphorus(QString name, Box *parent)
    : SupplyBase(name, parent)
{
    help("computes uptake supply of phosphorus");
    Input(gamma).unit("g P/g dw/molar P").help("Phosphorus uptake rate");
    Input(area).unit("m2/m").help("Leaf area");
    Input(waterP).unit("molar P").help("Water phosphorus concentration");
}

void SupplyPhosphorus::updateValue() {
    value = (demand > 0.) ? demand*(1. - exp(-gamma*area*waterP/demand)) : 0.;
}

}
