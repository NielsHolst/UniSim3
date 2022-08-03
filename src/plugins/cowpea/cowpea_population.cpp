#include <base/publish.h>
#include "cowpea_population.h"

using namespace base;

namespace cowpea {

PUBLISH(CowpeaPopulation)

CowpeaPopulation::CowpeaPopulation(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(initial).help("seed rate kg per hectare");
    Input(seedsSize).help("seeds size in mg");
    Input(established).help("percentage of established seed");
    Output(density);
}

void CowpeaPopulation::reset() {
    update();
}

void CowpeaPopulation::update() {
    density = initial*established/seedsSize;
}


}
