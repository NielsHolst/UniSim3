/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "distribution.h"

using namespace base;

namespace boxes {

PUBLISH(Distribution)

Distribution::Distribution(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes distribution fractiles");
    const QVector<int> q = {1, 5, 25, 50, 75, 95, 99};
    Input(input).help("the input port to anayse");
    Input(divisor).equals(100.).help("Divisor applied to `sections");
    Input(sections).equals(q).help("Sections applied at the end of every iteration");
    Input(steps).imports("/.[steps]");
}

void Distribution::amend() {
    int n = sections.size();
    _quantiles.fill(0., n);
    for (int i=0; i<n; ++i) {
        QString portName = name() + QString::number(sections.at(i));
        NamedOutput(portName, _quantiles[i]);
    }
}

void Distribution::reset() {
    _buffer.fill(0., steps+1);
    _ix = 0;
    _quantiles.fill(0.);
    update();
}

void Distribution::update() {
    _buffer[_ix++] = input;
}

void Distribution::cleanup() {
    std::sort(_buffer.begin(), _buffer.end(),
              [](double a, double b) { return a < b; });
    for (int i = 0; i < sections.size(); ++i) {
        int ix = static_cast<int>(std::floor(_buffer.size()*sections.at(i)/divisor));
        _quantiles[i] = _buffer.at(ix);
    }
}

} //namespace

