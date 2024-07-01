/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include <base/random_order.h>
#include "random_binomial.h"
#include "randomiser_base.h"

using namespace base;

namespace boxes {

PUBLISH(RandomBinomial)

RandomBinomial::RandomBinomial(QString name, Box *parent)
    : RandomBaseTyped<bool>(name, parent)
{
    help("produces random numbers from the binomial distribution");
    port("P")->equals(0.5).help("Probability of event");
    port("min")->help("Not used");
    port("max")->help("Not used");
}

void RandomBinomial::drawValue() {
    int stratum = _number->next(),
        numStrata = _number->size();
    double pFrom = double(stratum)/numStrata,
           pTo =   double(stratum+1)/numStrata;
    if (P < pFrom)
        value = false;
    else if (P > pTo)
        value = true;
    else
        value = randomiser()->draw01() < (P-pFrom)/(pTo-pFrom);
}

} //namespace
