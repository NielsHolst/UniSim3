/* Copyright 2019-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Teis Boderskov, Aarhus University [tebo atbios.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "demand_store.h"

using namespace base;

namespace saccharina {

PUBLISH(DemandStore)

DemandStore::DemandStore(QString name, Box *parent)
    : Density(name, parent)
{
    help("computes store's biomass demand");
    Input(propStore).unit("[0;1]").help("Store's proportion of total biomass");
    Input(propStructure).unit("[0;1]").help("Structure's proportion of total biomass");
    Input(concC).unit("g C/g dw").help("Carbon concentration");
    Input(concN).unit("g N/g dw").help("Nitrogen concentration");
    Input(concP).unit("g P/g dw").help("Phosphorus concentration");
    Input(dwStore).unit("g dw/m").help("Store's biomass");
    Input(dwStructure).unit("g dw/m").help("Structural biomass");
    Input(demandStructure).unit("g dw/m").help("Structure's demand");
}

void DemandStore::reset() {
   update();
}

void DemandStore::update() {
    dwStore = propStore/propStructure*(dwStructure + demandStructure) - dwStore;
    C = concC*dwStore;
    N = concN*dwStore;
    P = concP*dwStore;
}

}
