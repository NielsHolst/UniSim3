/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/publish.h>
#include "aphid_assimilate.h"

using namespace base;

namespace xpest {

PUBLISH(AphidAssimilate)

AphidAssimilate::AphidAssimilate(QString name, QObject *parent)
    : Damage(name, parent)
{
    Input(feedingRate).equals(0.5).help("Feeding rate per aphid mass").unit("g/g/d");
    Input(individualMass).equals(0.001).help("Average mass per aphid").unit("g");
    Input(density).equals(10.).help("Aphid density").unit("per m2");
}

void AphidAssimilate::update() {
    damage = feedingRate*individualMass*density;
}

} //namespace

