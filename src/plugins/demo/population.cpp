#include <base/publish.h>
#include "population.h"

using namespace base;

namespace demo {

PUBLISH(Population)

Population::Population(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(emigrationSlope).equals(20.).help("Slope of hyperbolic migration equation");
}

void Population::initialize() {
}



}
