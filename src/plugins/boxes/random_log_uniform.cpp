/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include <base/random_order.h>
#include "random_log_uniform.h"
#include "randomiser_base.h"

using namespace base;

namespace boxes {

PUBLISH(RandomLogUniform)

RandomLogUniform::RandomLogUniform(QString name, Box *parent)
    : RandomBaseTyped<double>(name, parent)
{
    help("produces random numbers from the log uniform distribution");
    port("min")->equals(0.01);
}

void RandomLogUniform::checkInputs(QString text) {
    RandomBaseTyped<double>::checkInputs(text);
    if (min <=0. || max <= 0.)
        ThrowException(text + ": Values for min and max must be positive for a log distribition").
                value(min). value2(max).context(this);
}

void RandomLogUniform::drawValue() {
    int stratum = _number->next(),
        numStrata = _number->size();
    double u = randomiser()->draw01(),
           w = (log(max)-log(min))/numStrata,
           log_value = log(min) + (stratum + u)*w;
    value = exp(log_value);
}

} //namespace
