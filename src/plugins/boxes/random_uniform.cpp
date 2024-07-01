/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include <base/random_order.h>
#include "random_uniform.h"
#include "randomiser_base.h"

using namespace base;

namespace boxes {

PUBLISH(RandomUniform)

RandomUniform::RandomUniform(QString name, Box *parent)
    : RandomBaseTyped<double>(name, parent)
{
    help("produces random numbers from the uniform distribution");
}

void RandomUniform::drawValue() {
    int stratum = _number->next(),
        numStrata = _number->size();
    double u = randomiser()->draw01(),
           w = (max-min)/numStrata;
    value = min + (stratum + u)*w;
}

} //namespace
