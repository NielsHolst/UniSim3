/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Merijn Moens, Aarhus University [merijnmoens at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/test_num.h>
#include "honey_store.h"

using namespace base;

namespace apis {

PUBLISH(HoneyStore)

HoneyStore::HoneyStore(QString name, QObject *parent)
    : Box(name, parent)
{
    help("holds colony honey store");
    Input(initial).equals(2).help("Initial store holding (kg)");
    Input(capacity).equals(40).help("Maximum holding capacity (kg)");
    Input(change).help("Change in honey store over one time step (kg)");
    Output(holding).help("Honey store holding (kg)");
}

void HoneyStore::reset() {
    holding = initial;
}

void HoneyStore::update() {
    holding += change;
    TestNum::snapToZero(holding);
}

}
