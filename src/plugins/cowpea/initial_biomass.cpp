#include <base/publish.h>
#include "initial_biomass.h"

using namespace base;

namespace cowpea {

PUBLISH(initialBiomass)

initialBiomass::initialBiomass(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(biomassPerPlant).help("fresh biomass per plant");
    Input(initial).help("number of plants");
    Output(value);
}

void initialBiomass::reset() {
    update();
}

void initialBiomass::update() {
    value = biomassPerPlant*initial;
}


}
