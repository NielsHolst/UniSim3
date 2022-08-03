/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mohammad Ali Mirhosseini, Tarbiat Modares University, Iran [mirhosseini1989 at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "Capacity.h"

using namespace base;

namespace tuta {

PUBLISH(Capacity)

Capacity::Capacity(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(pestNumber).help("pest population size");
    Input(K).help("carrying capacity of system");
    Output(factor).help("controlling factor for pest fecundity");
}

void Capacity::reset() {
    factor = 1;
}

void Capacity::update() {
        factor = (K - pestNumber)/K;
}


}
