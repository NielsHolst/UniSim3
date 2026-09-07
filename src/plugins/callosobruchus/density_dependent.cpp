#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "density_dependent.h"

using namespace base;

namespace callosobruchus {

PUBLISH(DensityDependent)

DensityDependent::DensityDependent(QString name, Box *parent)
    : Box(name, parent)
{
    Doc("models density-dependt survival and damage");
    Input(seedCapacity).equals(9).help("Number of beetles one seed can sustain");
    Input(beetlesProducedTotal).help("Total number of beetles produced since stocking");
    Input(seedsTotal).help("Total number of seeds stocked");
    Input(k).equals(8.17).help("Dispersion of the negative binomial");
    Input(exponent).equals(-0.496).help("Negative exponential response");
    Output(avgBeetlesProducedPerSeed).help("Average number of eggs per seed, laid since stocking");
    Output(survival).unit("[0;1]").help("Survival through juvenile life stages");
    Output(damage).unit("[0;1]").help("Proportion of seeds damaged");
}

void DensityDependent::reset() {
    update();
}

void DensityDependent::update() {
    avgBeetlesProducedPerSeed = beetlesProducedTotal/seedsTotal;
    if (avgBeetlesProducedPerSeed < 1e-6) {
        survival = 1.;
        damage = 0.;
        return;
    }

    _nbd.reset(avgBeetlesProducedPerSeed, k);
    const double P0 = _nbd.nextProbability();
    damage = 1. - P0;
    survival = 0;
    for (int i=1; i<seedCapacity; ++i) {
        const double P = surv(i)*_nbd.nextProbability()/(1. - P0);
        survival += P;
        if (P < 1e-6)
            break;
    }
    TestNum::snapTo(survival, 1.);
}

double DensityDependent::surv(int beetlesProducedTotalInSeed) const {
    return
        std::min(std::pow(9.*beetlesProducedTotalInSeed/seedCapacity, exponent), 1.);
}

} //namespace
