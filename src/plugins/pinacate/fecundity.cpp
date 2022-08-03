/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Andrew Antaya, University of Arizona [aantaya at email.arizona.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "fecundity.h"

using namespace base;

namespace pinacate {

PUBLISH(Fecundity)

Fecundity::Fecundity(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(density).help("number of adults currently alive");
    Input(percapita).help("reproductive output of each adult");
    Input(availability).help("vegetation availability limits population growth");

    Output(value);
}

void Fecundity::reset() {
}

void Fecundity::update() {
//calculate the number of new offspring produced
    value = density*percapita*availability;
}
}
