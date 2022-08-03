/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include <base/random_generator.h>
#include "randomiser_sobol_sequence.h"
#include "random_base.h"

using namespace base;

namespace boxes {

PUBLISH(RandomiserSobolSequence)

RandomiserSobolSequence::RandomiserSobolSequence(QString name, Box *parent)
    : RandomiserBase(name, parent), sobol(nullptr), variate(nullptr)
{
    help("draws quasi-random numbers [0;1) from the Sobol' sequence");
}

RandomiserSobolSequence::~RandomiserSobolSequence() {
    delete sobol;
    delete variate;
    sobol = nullptr;
    variate = nullptr;
}

void RandomiserSobolSequence::inializeGenerator() {
    delete sobol;
    delete variate;
    sobol = new boost::random::sobol(static_cast<unsigned int>(numVariables));
    variate = new Variate(*sobol, distribution);
}

void RandomiserSobolSequence::checkIterations(int theIterations) {
    int &k(numVariables);
    if (doSensitivityAnalysis) {
        int N = static_cast<int>(floor(log2(theIterations/double(2+k))));
        if (N<2)
            N = 2;
        int N1 = (2<<(N-1))*(2+k),
            N2 = (2<<N)*(2+k);
        QString msg("Simulation iterations must equal (2+k)*N (k=%1, N=2^i) to run a sensitivity analysis with a Sobol' sequence for random numbers"),
                hint("Use instead, for example, %1 or %2 iterations");
        if (theIterations!=N1 && theIterations!=N2) {
            ThrowException(msg.arg(k)).
                    value(theIterations).
                    hint(hint.arg(N1).arg(N2)).
                    context(this);
        }
    }
    else {
        int N = static_cast<int>(floor(log2(theIterations)));
        if (N<2)
            N = 2;
        int N1 = 2<<(N-1),
            N2 = 2<<N;
        QString hint("Use instead, for example, %1 or %2 iterations");
        if (theIterations!=N1 && theIterations!=N2 && !port("useFixed")->value<bool>()) {
            ThrowException("Simulation iterations must be a power of 2 when a Sobol' sequence is used for random numbers").
                    value(theIterations).
                    hint(hint.arg(N1).arg(N2)).
                    context(this);
        }
    }
}

int RandomiserSobolSequence::numStrata() const {
    return 1;
}

double RandomiserSobolSequence::draw01() {
    double test = (*variate)();
    return test;
}

}
