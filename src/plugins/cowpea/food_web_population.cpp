#include <base/publish.h>
#include "food_web_population.h"

using namespace base;

namespace cowpea {

PUBLISH(FoodWebPopulation)

FoodWebPopulation::FoodWebPopulation(QString name, QObject *parent)
    : FoodWebBoxBase(name, parent)
{
    Input(initial).equals(100).help("Initial density");
    Input(demandPerCap).help("Per capita demand rate");
}

void FoodWebPopulation::reset() {
    density = initial;
    demand = density*demandPerCap;
}

}
