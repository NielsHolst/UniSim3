/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <boost/math/distributions/negative_binomial.hpp>
#include <nbd.h>
#include "exception.h"

namespace base {
Nbd::Nbd() :
    _hasBeenReset(false)
{
}

Nbd::Nbd(double mu, double k) {
    reset(mu, k);
}

void Nbd::reset(double mu, double k) {
    _mu = mu;
    _k = k;
    const double p = k/(k + mu);
    boost::math::negative_binomial_distribution<> dist(k, p);
    _nextProbability = pdf(dist, 0);
    _x = 0;
    _hasBeenReset = true;
}

double Nbd::nextProbability() {
    if (!_hasBeenReset)
        ThrowException("Nbd object has not been reset(mu, k)");
    const double prob = _nextProbability;
    _nextProbability *= (_x + _k)/(_x + 1)*_mu/(_k + _mu);
    ++_x;
    return prob;
}

}
