/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "prey.h"
using namespace base;

namespace boxes {

PUBLISH(Prey)

Prey::Prey(QString name, Box *parent)
    : Box(name, parent) {
    help("defines the prey parameters for interaction with the parent Predator");
    Input(density).help("Density");
    Input(attackRate).help("Attack rate").unit("per predator per time unit");
    Input(gainFactor).equals(1.).help("Conversion of resource into supply units").unit("supply/resource");
    Output(loss).help("Amount of prey lost to predators");
    Output(mortality).help("Mortality caused by predators");
}

void Prey::setFoodWebImports() {
    Box *web = findOne<Box*>("../../pp");
    QString prefix = web->fullName(),
            suffix = "_" + name();
    port("loss")->imports(prefix + "[loss" + suffix + "]");
    port("mortality")->imports(prefix + "[mortality" + suffix + "]");
}

} //namespace

