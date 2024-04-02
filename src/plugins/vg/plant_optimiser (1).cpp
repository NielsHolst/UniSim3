/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "plant.h"
#include "plant_optimiser.h"

using namespace base;

namespace vg {

PUBLISH(PlantOptimiser)

Plant *PlantOptimiser::plant;

PlantOptimiser::PlantOptimiser(QString name, Box *parent)
    : Box(name, parent)
{
    help("optimises plant growth");
    Input(refTemperature).equals(25.).unit("oC").help("Reference value for comparison");
    Input(refCo2).equals(1800.).unit("ppm").help("Reference value for comparison");
    Output(growthRate).unit("g carbohydrate/m2 ground").help("Optimal carbohydrate biomass produced in this time step");
    Output(performance).unit("[0;1]").help("Growth rate performance compared to reference");
}

void PlantOptimiser::initialize() {
    plant = findOne<Plant*>("*");
    plantGrowthRate = plant->port("growthRate")->valuePtr<double>();
}

void PlantOptimiser::reset() {
    update();
}

void PlantOptimiser::update() {
    plant->saveState();
    growthRate = plant->calcGrowthRate(refTemperature, refCo2);
    plant->restoreState();
    performance = (*plantGrowthRate > 0) ? std::min(growthRate/(*plantGrowthRate), 1.) : 0.;
}

} //namespace

