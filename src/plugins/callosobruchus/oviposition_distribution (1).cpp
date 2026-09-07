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
    Input(parasitised0).help("No. of undamaged seeds");
    Input(parasitised1).help("No. of seeds parasitised once");
    Input(parasitised2).help("No. of seeds parasitised more than once");
    Output(undamagedTo1).help("No. of undamaged seeds -> parasitised once");
    Output(undamagedTo2).help("No. of undamaged seeds -> parasitised more than once");
    Output(parasitised1To2).help("No. of seeds parasitised once -> more than once");
}

void OvipositionDistribution::reset() {
}

void OvipositionDistribution::update() {
    // Enumerate to whole eggs
    long int eggsLeft = (long int) (numEggsLaid + _carryOver);
    _carryOver = snapToZero(numEggsLaid - eggsLeft);

    long int
        parasitised0 = (long int) OvipositionDistribution::parasitised0,
        parasitised1 = (long int) OvipositionDistribution::parasitised1,
        parasitised2 = (long int) OvipositionDistribution::parasitised2;

    for (int i=eggsLeft; i>0; --i) {
        // Compute probabilites
        const double
            w0 = p0*parasitised0,
            w1 = p1*parasitised1,
            w2 = p2*parasitised2,
            sum = snapToZero(w0 + w1 + w2),
            prob0 = w0/sum,
            prob1 = w1/sum;
        // Place egg
        // Place egg in unparasitied seed?
        if (u() < prob0) {
            --parasitised0;
            ++parasitised1;
        }
        // Place egg in seed parasitised once before?
        else if (u() < prob1/(1. - prob0)) {
            --parasitised1;
            ++parasitised2;
        }
        // else egg is placed in parasitised2, which does not change its status
    }
}


} //namespace
