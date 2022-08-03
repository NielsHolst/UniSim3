#include <base/publish.h>
#include "prey.h"

using namespace base;

namespace demo {

PUBLISH(Prey)

Prey::Prey(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(r).equals(0.05).help("Intrinsic rate of increase (per day)");
}


void Prey::reset() {
    density = initial;
}

void Prey::update() {
    rActual = r*(K-totalPrey)/K;
    density += rActual*density*timeStep - loss;
}


}
