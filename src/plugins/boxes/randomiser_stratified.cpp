/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include <base/random_generator.h>
#include "randomiser_stratified.h"
#include "random_base.h"

using namespace base;

namespace boxes {

PUBLISH(RandomiserStratified)

RandomiserStratified::RandomiserStratified(QString name, Box *parent)
    : RandomiserBase(name, parent), variate(nullptr)
{
    help("draws random numbers [0;1) by stratification");
}

RandomiserStratified::~RandomiserStratified() {
    delete variate;
    variate = nullptr;
}

void RandomiserStratified::inializeGenerator() {
    delete variate;
    variate = new Variate(randomGenerator(), distribution);
}

void RandomiserStratified::checkIterations(int iterations) {
    if (doSensitivityAnalysis)
        checkIterationsSimple(iterations);
}

int RandomiserStratified::numStrata() const {
    return doSensitivityAnalysis ? N : iterations;
}

double RandomiserStratified::draw01() {
    return (*variate)();
}

}
