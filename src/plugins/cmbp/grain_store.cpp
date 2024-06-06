/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** William Meikle, USDA [william.meikle at usda.gov].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "grain_store.h"

using namespace base;

namespace cmbp {

PUBLISH(GrainStore)

GrainStore::GrainStore(QString name, Box *parent)
    : Box(name, parent)
{
    Input(Tmin).help("Outdoors daily minimum temperature (oC)");
    Input(Tmax).help("Outdoors daily maximum temperature (oC)");
    Input(Toffset).help("Offset for temperature inside the store (oC)");
    Input(mass).equals(500).help("Mass of stored grain (kg)");
    Output(T).help("Store temperature (oC");
}

void GrainStore::reset() {
    update();
}

void GrainStore::update() {
    T = (Tmin + Tmax)/2. + Toffset;
}


}
