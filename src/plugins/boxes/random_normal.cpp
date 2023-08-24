/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/random_order.h>
#include "random_normal.h"
#include "randomiser_base.h"

#include <base/dialog.h>

using namespace base;

namespace boxes {

PUBLISH(RandomNormal)

RandomNormal::RandomNormal(QString name, Box *parent)
    : RandomBaseTyped<double>(name, parent)
{
    help("produces random numbers from the normal distribution");
    port("P")->equals(0).help("Not used");
}


void RandomNormal::drawValue() {
    int stratum = _number->next(),
        numStrata = _number->size();
    double u = randomiser()->draw01(),
           w = 1./numStrata,
           u2 = (stratum + u)*w;
    value = phys_math::invNormalRange(u2, min, max, P);
}

} //namespace
