/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "reserves.h"

using namespace base;

namespace saccharina {

PUBLISH(Reserves)

Reserves::Reserves(QString name, QObject *parent)
    : Box(name, parent)
{
    help("keeps track of reserves (C or N) as proportion of structural mass");
    Input(initialProportion).unit("[0;1]").help("Initial proportion");
    Input(maxProportion).unit("[0;1]").help("Max proportion of reserves");
    Input(allocatedProportion).unit("[0;1]").help("Proportion after allocation of supplies");
    Output(proportion).unit("[0;1]").help("Proportion of reserves relative to structural mass");
}

void Reserves::reset() {
    proportion = initialProportion;
}

void Reserves::update() {
    proportion = allocatedProportion;
}

}
