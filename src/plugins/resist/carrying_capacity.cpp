/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Christian Nansen, University of California [chrnansen at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "carrying_capacity.h"

using namespace base;

namespace resist {

PUBLISH(CarryingCapacity)

CarryingCapacity::CarryingCapacity(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(value).help("The carrying capacity for all genotypes total");
}

}
