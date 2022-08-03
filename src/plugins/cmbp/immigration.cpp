/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** William Meikle, USDA [william.meikle at usda.gov].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "immigration.h"

using namespace base;

namespace cmbp {

PUBLISH(Immigration)

Immigration::Immigration(QString name, Box *parent)
    : Box(name, parent)
{
    Input(number).help("Nuber of P.t. arriving");
    Input(storeMass).equals(500).help("Size of store (kg maize)");
    Input(day).help("Day of immigration (no. of days after start)");
    Output(value).help("Immigrant increase in P.t. density (per kg");
}

void Immigration::reset() {
    _day = 0;
    _happened = false;
}

void Immigration::update() {
    bool passedImmigrationDay = (_day++>=day);
    value = (passedImmigrationDay && !_happened)  ? number/storeMass : 0;
    _happened = passedImmigrationDay;
}


}
