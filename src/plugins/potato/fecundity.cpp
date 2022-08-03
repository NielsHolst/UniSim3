/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Isaac Kwesi Abuley , Aarhus University [ikabuley at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "fecundity.h"

using namespace base;

namespace potato {

PUBLISH(Fecundity)

Fecundity::Fecundity(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(R0).equals(170).help("Life time net fecundity [eggs per female]");
    Input(senescence).imports("potato/tuberization[outflowTotal]");
    Output(value).help("Life time fecundity [0;1]");
}

void Fecundity::reset() {
    update();
}

void Fecundity::update() {
    value = R0*(1. - senescence);
}

}
