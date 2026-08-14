#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "oviposition_distribution.h"

using namespace base;
using TestNum::gt;
using TestNum::snapToZero;

namespace callosobruchus {

PUBLISH(OvipositionDistribution)

OvipositionDistribution::OvipositionDistribution(QString name, Box *parent)
    : Box(name, parent)
{
    help("distributes eggs laid among seeds parasitised 0, 1 or more times");
    Input(numEggsLaid).help("Number of eggs laid");
    Input(p0).unit("[0;1]").help("Preference for undamaged seeds");
    Input(p1).unit("[0;1]").help("Preference for seeds parasitised once");
    Input(p2).unit("[0;1]").help("Preference for seeds parasitised more than once");
    Input(parasitised0).help("No. of undamaged seeds");
    Input(parasitised1).help("No. of seeds parasitised once");
    Input(parasitised2).help("No. of seeds parasitised more than once");
    Output(p0To1).help("No. of undamaged seeds -> parasitised once");
    Output(p0To2).help("No. of undamaged seeds -> parasitised more than once");
    Output(p1To2).help("No. of seeds parasitised once -> more than once");
}

void OvipositionDistribution::reset() {
}

void OvipositionDistribution::update() {
    // Compute weighted probalities
    const double
        w0 = snapToZero(p0*parasitised0),
        w1 = snapToZero(p1*parasitised1),
        w2 = snapToZero(p2*parasitised2),
        sum = snapToZero(w0 + w1 + w2),
        P0 = w0/sum,
        P1 = w1/sum,
        eggs0 = P0*numEggsLaid,
        eggs1 = P1*numEggsLaid;
        // eggs2 is not needed
    // Distribute eggs0
    OvipositionDistribution::Poisson pois0, pois1;
    if (eggs0 > 0.) {
        pois0 = poisson(eggs0/parasitised0);
        p0To1 = snapToZero(pois0.one)*parasitised0;
        p0To2 = snapToZero(pois0.more)*parasitised0;
    }
    else {
        p0To1 = p0To2 = 0.;
    }
    if (eggs1 > 0.) {
        pois1 = poisson(eggs1/parasitised1);
        p1To2 = snapToZero(pois1.one + pois1.more)*parasitised1;
    }
    else {
        p1To2 = 0.;
    }
    if (gt(p0To1 + p0To2, parasitised0))
        ThrowException("Unexpected inequality; too may eggs laid").value1(p0To1 + p0To2 + p1To2).value2(numEggsLaid);
    if (gt(p1To2, parasitised1))
        ThrowException("Unexpected inequality; too may eggs laid in par1").value1(p1To2).value2(parasitised1);
}

OvipositionDistribution::Poisson OvipositionDistribution::poisson(double average) {
    const double
        zero = exp(-average),
        one = average*zero;
    return Poisson{one, 1. - zero - one};
}

} //namespace
