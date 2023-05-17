/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "budget_volume_external.h"

using namespace base;

namespace vg {

PUBLISH(BudgetVolumeExternal)

BudgetVolumeExternal::BudgetVolumeExternal(QString name, base::Box *parent)
    : BudgetVolume(name, parent)
{
}

void BudgetVolumeExternal::update() {
}


}
