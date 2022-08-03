/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Isaac Kwesi Abuley , Aarhus University [ikabuley at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "survival.h"

using namespace base;

namespace potato {

PUBLISH(Survival)

Survival::Survival(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(senescence).imports("potato/tuberization[outflowTotal]");
    Output(value).help("Life time survival [0;1]");
}

void Survival::reset() {
    update();
}

void Survival::update() {
    value =  1. - senescence;
}

}
