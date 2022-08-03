/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/publish.h>
#include "weeds.h"

using namespace base;

namespace xpest {

PUBLISH(Weeds)

Weeds::Weeds(QString name, QObject *parent)
    : Damage(name, parent)
{
    Input(density).equals(10.).help("Weed density (per m2)");
    Input(exponent).equals(0.003).help("Parameter for the negative exponential function");
}

void Weeds::update() {
    damage = 1. - exp(-exponent*density);
}

} //namespace

