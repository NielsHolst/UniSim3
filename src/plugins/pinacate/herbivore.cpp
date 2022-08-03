/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Andrew Antaya, University of Arizona [aantaya at email.arizona.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "herbivore.h"

using namespace base;

namespace pinacate {

PUBLISH(Herbivore)

Herbivore::Herbivore(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(weight).help("weight in kg of each indv");
    Input(popsize).help("number of individuals currently alive in the specified stage");

    Output(value); //this value will be in kg herbivore
}

void Herbivore::reset() {
}

void Herbivore::update() {
//calculate the biomass of herbivore
    value = (weight*popsize);
}


}
