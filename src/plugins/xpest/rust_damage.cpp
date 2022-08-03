/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/publish.h>
#include "rust_damage.h"

using namespace base;

namespace xpest {
	
PUBLISH(RustDamage)

RustDamage::RustDamage(QString name, QObject *parent)
    : Damage(name, parent)
{
    Input(pestSeverity).equals(1.).help("Severity of pest").unit("%");
    Input(lai).equals(0.).help("Leaf area index").unit("m2/m2");
    Input(pustuleArea).equals(1e-6).help("Area on one pustule").unit("m2");
    Input(pustuleDemand).equals(4.62e-6).help("Assimilate demand of one pustule").unit("g/d");
}

void RustDamage::update() {
    damage = pestSeverity/100. * lai/pustuleArea * pustuleDemand;
}

} //namespace

