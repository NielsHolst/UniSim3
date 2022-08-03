/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/publish.h>
#include "aphid_rue.h"

using std::min;
using namespace base;

namespace xpest {
	
PUBLISH(AphidRue)

AphidRue::AphidRue(QString name, QObject *parent)
    : Damage(name, parent)
{
    Input(assimilateEaten).equals(0.).help("Amount of assimilate eaten").unit("(g/m2/d)");
    Input(honeyProportion).equals(0.35).help("Proportion of honey produced from assimilates").unit("[0;1]");
    Input(dewProportion).equals(0.015).help(" Rue reduction by honey").unit("(m2/g)]");
    Input(maxRueReduction).equals(0.2).help("Max. reduction in RUE").unit("[0;1]");
    Output(honey).help("Total amount of honey produced").unit("(g/m2)");
}

void AphidRue::update() {
    honey += honeyProportion*assimilateEaten;
    damage = min(dewProportion*honey, maxRueReduction);
}

} //namespace

