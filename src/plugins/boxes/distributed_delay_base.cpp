/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box.h>
#include "distributed_delay_base.h"

namespace boxes {

DistributedDelayBase::DistributedDelayBase(base::Box *parent)
    : _xSum(0), _parent(parent)
{
}

void DistributedDelayBase::scale(double factor) {
    if (factor == 0.) {
        _x.fill(0.);
        _xSum = 0.;
    }
    else if (factor != 1.) {
        int n = _x.size();
        double *v = _x.data();
        for (int i = 0; i < n; ++i, ++v) *v *= factor;
        _xSum *= factor;
    }
}

const QVector<double>& DistributedDelayBase::take(double proportion) {
    if (proportion == 0.) {
        taken.fill(0., _x.size());
    }
    if (proportion == 1.) {
        taken = _x;
        _x.fill(0.);
        _xSum = 0.;
    }
    else {
        int n = _x.size();
        taken.resize(n);
        double *source = _x.data();
        double *destination = taken.data();
        for (int i = 0; i < n; ++i, ++source, ++destination) {
            *destination = *source*proportion;
            *source -= *destination;
        }
        _xSum *= (1. - proportion);
    }
    return taken;
}

void DistributedDelayBase::put(const QVector<double> &addition) {
    const double *source = addition.data();
    double *destination = const_cast<double*>(_x.data());
    _xSum = 0.;
    int n = _x.size();
    for (int i = 0; i < n; ++i, ++source, ++destination) {
        _xSum = *destination += *source;
    }
}

QVector<double> &DistributedDelayBase::data() {
    return _x;
}

double DistributedDelayBase::content() const {
    return _xSum;
}

} // namespace
