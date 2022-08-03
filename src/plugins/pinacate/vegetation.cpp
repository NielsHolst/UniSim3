/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Andrew Antaya, University of Arizona [aantaya at email.arizona.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "vegetation.h"

using namespace base;

namespace pinacate {

PUBLISH(Vegetation)

Vegetation::Vegetation(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(herbivorebiomass).help("the total sum of deer and sheep biomass");
    Input(K).help("carrying capacity in kg of herbivore biomass");
    Input(scaling).help("random draw from a uniform distribution that models yearly rainfall, which influences vegetation availability");
    Input(currentday).help("the current day of the in the simulation");

    Output(availability);
}

void Vegetation::reset() {
}

void Vegetation::update() {
// calculate the availability of vegetation (food) based on carrying capacity
   availability = (scaling*((K-herbivorebiomass)/K));
   if (availability < 0)
   availability = 0;
   Box *scalingBox = findOne<Box*>("./scaling");
   if (currentday == 1)
           scalingBox -> reset();
}

}

