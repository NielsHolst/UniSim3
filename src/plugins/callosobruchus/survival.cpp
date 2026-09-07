#include <base/exception.h>
#include <base/publish.h>
#include "survival.h"

using namespace base;

namespace callosobruchus {

PUBLISH(Survival)

Survival::Survival(QString name, Box *parent)
    : Box(name, parent)
{
    Doc("models survival of immature stages");
    Input(seedCapacity).equals(9).help("Number of immatures one seed can sustain");
    Input(eggsTotal).help("Total number of eggs laid since stocking");
    Input(seedsTotal).help("Total number of seeds stocked");
    Input(k).equals(8.17).help("Dispersion of the negative binomial");
    Input(exponent).equals(-0.496).help("Negative exponential response to egg density in seed");
    Output(avgEggsPerSeed).help("Average number of eggs per seed, laid since stocking");
    Output(value).help("Survival from egg to adult");
}

void Survival::reset() {
    update();
}

void Survival::update() {
    avgEggsPerSeed = eggsTotal/seedsTotal;
    if (eggsTotal == 0) {
        value = 1.;
        return;
    }

    _nbd.reset(avgEggsPerSeed, k);
    const double P0 = _nbd.nextProbability();
    value = 0;
    for (int i=1; i<seedCapacity; ++i) {
        const double P = survival(i)*_nbd.nextProbability()/(1. - P0);
        value += P;
        if (P < 1e-6)
            break;
    }
}

double Survival::survival(int numEggsInSeed) const {
    return
        std::min(std::pow(9.*numEggsInSeed/seedCapacity, exponent), 1.);
}

} //namespace
