/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mohammad Ali Mirhosseini, Tarbiat Modares University, Iran [mirhosseini1989 at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "Tomato.h"

using namespace base;

namespace tuta {

PUBLISH(Tomato)

Tomato::Tomato(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(pestNumber).help("number of pest larva");
    Input(damageRate).help("damage per larvae per day");
    Input(growthRate).help("tomato growth rate");
    Output(status).help("tomato status");
}

void Tomato::reset() {
    status = 1;
}

void Tomato::update() {
    double loss = pestNumber * damageRate;
    double gain = growthRate * status * (1-status);
    status += gain - loss;

}


}
