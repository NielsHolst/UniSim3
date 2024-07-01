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
    : Box(name, parent)
{
    help("computes store's elementary demand");
    Input(element).unit("[C|N|P]").help("One letter denoting kind of store");
    Input(concentration).unit("g element/g dw").help("Store elementary concentration");
    Input(idealStructure).imports("proportions[idealStructure]");
    Input(idealStore).unit("[0;1]").help("Ideal proportion of dry weight in the store");
    Input(dwWhole).unit("g dw/m").help("Whole plant biomass");
    Input(dwStore).unit("g dw/m").help("Store's biomass");
}

void DemandStore::amend() {
    if (element!="C" && element!="N" && element!="P")
        ThrowException("'element' must be C, N or P").value(element).context(this);
    // 'value' will be given the port name "C", "N" or "P" defined by 'element'
    QString u = "g " + element + "/m";
    NamedOutput(element, value).unit(u).help("Elementary demand");
}

void DemandStore::reset() {
   update();
}

void DemandStore::update() {
    value = std::max(idealStore*dwWhole - dwStore, 0.)*concentration;
}

}

