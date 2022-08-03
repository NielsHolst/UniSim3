/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/mega_factory.h>
#include <base/publish.h>
#include "food_web.h"
#include "host.h"
#include "parasite.h"
#include "predator.h"
#include "prey.h"

#include <base/dialog.h>

using namespace base;

namespace boxes {

PUBLISH(FoodWeb)

FoodWeb::FoodWeb(QString name, Box *parent)
    : Box(name, parent)  {
    help("computes food web acquisition among Predator/Prey and Parasite/Host objects");
    Input(timeStep).equals(1.).help("Simulation time step");
}

void FoodWeb::amend() {
    Box *pp = MegaFactory::create<>("FoodWebCalcPredatorPrey", "pp", this),
        *ph = MegaFactory::create<>("FoodWebCalcParasiteHost", "ph", this);
    pp->amendFamily();
    ph->amendFamily();
    setFoodWebImports();
}

void FoodWeb::setFoodWebImports() {
    QVector<Host*> hosts = findMany<Host*>("descendants::*");
    for (Host *host : hosts)
        host->setFoodWebImports();

    QVector<Parasite*> parasites = findMany<Parasite*>("descendants::*");
    for (Parasite *parasite : parasites)
        parasite->setFoodWebImports();

    QVector<Predator*> predators = findMany<Predator*>("descendants::*");
    for (Predator *predator : predators)
        predator->setFoodWebImports();

    QVector<Prey*> prey = findMany<Prey*>("descendants::*");
    for (Prey *aPrey : prey)
        aPrey->setFoodWebImports();
}

} //namespace
