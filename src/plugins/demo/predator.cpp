#include <base/publish.h>
#include "predator.h"

using namespace base;

namespace demo {

PUBLISH(Predator)

Predator::Predator(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(r).equals(20.).help("Slope of hyperbolic migration equation");
}

void Predator::initialize() {
}



}
