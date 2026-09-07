#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "oviposition_distribution.h"

using namespace base;
using TestNum::snapToZero;

namespace callosobruchus {

PUBLISH(OvipositionDistribution)

OvipositionDistribution::OvipositionDistribution(QString name, Box *parent)
    : Box(name, parent)
{
    Doc("distributes eggs laid among seeds parasitised 0, 1 or more times");
    Input(numEggsLaid).help("Number of eggs laid");
    Input(p0).unit("[0;1]").help("Preference for undamaged seeds");
    Input(p1).unit("[0;1]").help("Preference for seeds parasitised once");
    Input(p2).unit("[0;1]").help("Preference for seeds parasitised more than once");
    Input(undamaged).help("No. of undamaged seeds");
    Input(parasitised1).help("No. of seeds parasitised once");
    Input(parasitised2).help("No. of seeds parasitised more than once");
    Output(undamagedTo1).help("No. of undamaged seeds -> parasitised once");
    Output(undamagedTo2).help("No. of undamaged seeds -> parasitised more than once");
    Output(parasitised1To2).help("No. of seeds parasitised once -> more than once");
}

void OvipositionDistribution::reset() {
}

void OvipositionDistribution::update() {
    const double
        w0 = p0*undamaged,
        w1 = p1*parasitised1,
        w2 = p2*parasitised2,
        sum = snapToZero(w0 + w1 + w2),
        eggs0 = (sum > 0.) ? numEggsLaid*w0/sum : 0.,
        eggs1 = (sum > 0.) ? numEggsLaid*w1/sum : 0.,
        avg0 = (undamaged > 0.) ? eggs0/undamaged : 0.;

    // Distribute eggs among undamaged seeds
    if (avg0 > 0.) {
        const auto pois = truncatedPoisson(avg0);
        undamagedTo1 = snapToZero(pois.one*eggs0);
        undamagedTo2 = snapToZero(pois.more*eggs0);
    }
    else {
        undamagedTo1 = undamagedTo2 = 0.;
    }

    // Distribute eggs among once damaged seeds
    // (it doesn't matter how many eggs are laid)
    const auto pois = poisson(avg0);
    parasitised1To2 = snapToZero(pois.oneOrMore*eggs1);
}

OvipositionDistribution::Poisson OvipositionDistribution::poisson(double average) {
    const double zero = exp(-average);
    return Poisson{zero, 1. - zero};
}

OvipositionDistribution::TruncatedPoisson OvipositionDistribution::truncatedPoisson(double average) {
    const double
        zero = exp(-average),
        one = average*zero,
        rescale = 1. - zero;
    return TruncatedPoisson{one/rescale, (1. - zero - one)/rescale};
}

} //namespace
