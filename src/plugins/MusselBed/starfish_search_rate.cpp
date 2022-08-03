/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/

/* ##STARFISH SEARCH RATE MODEL: determines the apparency or search rate for starfish to be considered in the functional response. The empirical value of 0.17
** from Barbeau and Wong is evaluated against field current environmental conditions that difficult starfish activity and movement*/

#include <base/publish.h>
#include "starfish_search_rate.h"

using namespace base;

namespace MusselBed {

PUBLISH(StarfishSearchRate)

StarfishSearchRate::StarfishSearchRate(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(s).equals(0.17).help("maximum apparency under optimal conditions");
    Output(value).help("apparency of prey under current stress conditions");
}

void StarfishSearchRate::initialize() {
    scales = findMany<Box*>("scales/*");
}

void StarfishSearchRate::reset() {
    update();
}

void StarfishSearchRate::update() {
    value = s;
    for (int i = 0; i < scales.size(); ++i) {
        value *= scales[i]->port("value")->value<double>();
    }
}

} //namespace
