/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/gradient_ascent.h>
#include "plant.h"
#include "plant_instrument.h"

using namespace base;

namespace vg {

PUBLISH(PlantInstrument)


PlantInstrument::PlantInstrument(QString name, Box *parent)
    : Box(name, parent)
{
    help("instrumentalises a Plant object");
    Input(netRadiation).unit("W/m2").help("Net radiation of plant canopy");
    Input(parAbsorbed).unit("&micro;mol/m2/s").help("PAR absorbed by plant canopy");
}

void PlantInstrument::initialize() {
    plant = findOne<Plant*>("..");
}

void PlantInstrument::reset() {
    update();
}

void PlantInstrument::update() {
    plant->updateByRadiation(netRadiation, parAbsorbed);
}

} //namespace

