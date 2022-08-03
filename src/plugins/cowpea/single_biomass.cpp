#include <base/publish.h>
#include "single_biomass.h"

using namespace base;

namespace cowpea {

PUBLISH(SingleBiomass)

SingleBiomass::SingleBiomass(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(initial).help("plants per square meter");
    Input(expectedBiomass).help("expected fresh biomass in g per square meter");
    Output(single);
}

void SingleBiomass::reset() {
    update();
}

void SingleBiomass::update() {
    single = expectedBiomass/initial;
}


}
