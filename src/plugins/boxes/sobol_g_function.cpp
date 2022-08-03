/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "sobol_g_function.h"

using namespace base;

namespace boxes {

PUBLISH(SobolGFunction)

SobolGFunction::SobolGFunction(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes Sobol' g function");
    Input(a).help("Vector of parameters");
    Input(x).unit("[0;1]").help("Vector of inputs");
    Output(value).help("B value");
}

void SobolGFunction::initialize() {
    if (a.size() != x.size())
        ThrowException("Vectors a and x must be of same size").value1(a.size()).value2(x.size()).context(this);
}

void SobolGFunction::reset() {
  update();
}

void SobolGFunction::update() {
    value = 1.;
    for (auto[ai,xi] = std::pair(a.begin(), x.begin()); ai!=a.end(); ++ai, ++xi) {
        value *= (fabs(4*(*xi)-2) + *ai)/(1. + *ai);
    }
}

}
