/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "supply_resorption.h"

using namespace base;

namespace saccharina {

PUBLISH(SupplyResorption)

SupplyResorption::SupplyResorption(QString name, Box *parent)
    : Box(name, parent)
{
    help("calculates amount of carbon and nitrogen resorbed");
    Input(propCResorbed).equals(0.4).unit("[0;1]").help("Proportion of carbon resorbed");
    Input(propNResorbed).equals(0.6).unit("[0;1]").help("Proportion of nitrogen resorbed");
    Input(senescentBiomass).imports("biomassStruct[outflow]");
    Input(propCStruct).imports("plant[propCStruct]");
    Input(propNStruct).imports("plant[propNStruct]");
    Output(carbon).unit("g C/m").help("Carbon resorbed");
    Output(nitrogen).unit("g N/m").help("Nitrogen resorbed");
}

void SupplyResorption::reset() {
   update();
}

void SupplyResorption::update() {
    carbon   = propCResorbed*propCStruct*senescentBiomass;
    nitrogen = propNResorbed*propNStruct*senescentBiomass;
}

}
