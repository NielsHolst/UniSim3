/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/dialog.h>
#include <base/publish.h>
#include "parasite.h"
using namespace base;

namespace boxes {

PUBLISH(Parasite)

Parasite::Parasite(QString name, Box *parent)
    : Box(name, parent) {
    help("defines the demand and supply of a parasite");
    Input(density).help("Density");
    Input(demand).help("Amount of resource needed");
    Output(supply).help("Total amount of resource acquired");
    Output(sdRatio).help("Total supply/demand ratio").unit("[0;1]");
}

void Parasite::setFoodWebImports() {
    Box *web = findOne<Box*>("../pp");
    QString prefix = web->fullName(),
            suffix = "_" + name();
    port("supply")->imports(prefix + "[supply" + suffix + "]");
    port("sdRatio")->imports(prefix + "[sdRatio" + suffix + "]");
}

} //namespace

