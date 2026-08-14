/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include <algorithm>
#include <base/publish.h>
#include "exponential.h"

using namespace base;
using std::max;
using std::min;

namespace eq {

PUBLISH(Exponential)

Exponential::Exponential(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates y-increments for given x-increments along an exponential growth curve");
    Input(y).help("Current y-values");
    Input(r).equals(0).help("Exponential growth rate (per dt) of any y");
    Input(dt).help("Time step for any y");
    Input(yMax).equals(1e100).help("The maximum value of any y");
    Output(dy).help("By how much will y increase over the time step (dx)? Will match the size of y" );
}

void Exponential::reset() {
    dy.resize(y.size());
}

double Exponential::fdy(double y) {
    double dy = y*(exp(r*dt)-1),
           dyMax = yMax - y;
    return (dy > dyMax) ? dyMax : dy;
}

void Exponential::update() {
    std::transform(y.constBegin(), y.constEnd(), dy.begin(), [this](const double &y){return this->fdy(y);});
}
}
