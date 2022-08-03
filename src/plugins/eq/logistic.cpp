/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "logistic.h"

using namespace base;

namespace eq {

PUBLISH(Logistic)

Logistic::Logistic(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes logistic growth in population density");
    Input(x).help("Population density");
    Input(r).help("Intrinsic rate of increase");
    Input(K).help("Carrying capacity");
    Input(timeStep).equals(1).help("Carrying capacity");
    Output(value).help("Population increment");
}

void Logistic::update() {
    double &y0(x);
    double l = exp(r*timeStep);
    double y = K*y0*l/(K + y0*(l-1));
    value = y - y0;
}

} //namespace

