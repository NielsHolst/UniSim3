/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Emily Bick, University of California [enbick at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "intervention.h"

using namespace base;

namespace vacuum {

PUBLISH(Intervention)

Intervention::Intervention(QString name, QObject *parent)
    : Box(name, parent)
{
    help("scales a value by mortality");
    Input(dayValue).equals(1).help("Number of Days)");
    Input(mortality).equals(0.0).help("returned mortality value");
    Input(interval).equals(4).help("intervention run every X days");
    Input(efficacy).equals(.2).help("Value of intervention event mortality");
    //Output(value).help("Scaled value for mortality");
}

void Intervention::reset() {
    update();
}

void Intervention::update() {
    if (dayValue%interval == 0)
    {
          mortality = efficacy;
    }
    else
    {
          mortality = 0;
    }

}


}
