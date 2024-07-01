/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include <base/gradient_ascent.h>
#include "plant.h"
#include "plant_optimiser.h"

using namespace base;

namespace vg {

PUBLISH(PlantOptimiser)

Plant *PlantOptimiser::plant;

double optimiserFunction(double temperature, double co2) {
    return PlantOptimiser::plant->calcGrowthRate(temperature, co2);
}

PlantOptimiser::PlantOptimiser(QString name, Box *parent)
    : Box(name, parent), optimiser(optimiserFunction)
{
    help("optimises plant growth");
    Input(minTemperature).equals(15.).unit("oC").help("Lower bound on optimisation");
    Input(maxTemperature).equals(30.).unit("oC").help("Upper bound on optimisation");
    Input(minCo2).equals(300.).unit("ppm").help("Lower bound on optimisation");
    Input(maxCo2).equals(1800.).unit("ppm").help("Upper bound on optimisation");
    Input(maxIterations).equals(30).help("Max. iterations for optimisation");
    Input(delta).equals(1e-3).help("Delta for gradient computation");
    Output(temperature).unit("oC").help("Optimal temperature for growth");
    Output(co2).unit("ppm").help("Optimal CO2 for growth");
    Output(growthRate).unit("g carbohydrate/m2 ground").help("Optimal carbohydrate biomass produced in this time step");
    Output(iterations).help("Number of iterations used to determine optimum");
}

void PlantOptimiser::initialize() {
    plant = findOne<Plant*>("..");
    optimiser.
        bracketX(minTemperature, maxTemperature).
        bracketY(minCo2, maxCo2).
        delta(delta).
        maxIterations(maxIterations);
}

void PlantOptimiser::reset() {
    temperature = (minTemperature + maxTemperature)/2.;
    co2 = (minCo2 + maxCo2)/2.;
    update();
}

void PlantOptimiser::update() {
    plant->saveState();
    optimiser.init(temperature, co2);
    auto solution = optimiser.solve();
    temperature = solution.x;
    co2 = solution.y;
    iterations = solution.iterations;
    plant->restoreState();
}

} //namespace

