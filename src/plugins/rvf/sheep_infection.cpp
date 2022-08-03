/* Copyright 2012-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Clement N. Mweya, National Institute for Medical Research, Tukuyu, Tanzania [cmweya at nimr.or.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "sheep_infection.h"

using namespace base;

namespace rvf {

PUBLISH(SheepInfection)

SheepInfection::SheepInfection(QString name, QObject *parent)
     : Box(name, parent)
{
    Input(mortalityRate).help("Sheep death rate due to disease per day");
    Input(density).help("Density of infected hosts");
    Output(mortality).help("Number died");
}

void SheepInfection::update() {
    mortality = mortalityRate*density;
}
} //namespace
