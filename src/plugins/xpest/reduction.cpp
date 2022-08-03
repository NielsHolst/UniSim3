/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/publish.h>
#include "reduction.h"

using namespace base;

namespace xpest {
	
PUBLISH(Reduction)

Reduction::Reduction(QString name, QObject *parent)
    : Damage(name, parent)
{
    Input(pestSeverity).equals(1.).help("Severity of pest (%)");
    Input(maxDamage).equals(0.35).help("Max. reduction caused");
    Input(beta).equals(1).help("Exponent to scale for leaf area effect");
    Input(damages).imports("./*[damage]");
}

void Reduction::update() {
    if (damages.isEmpty()) {
        damage = pow(maxDamage*pestSeverity/100., beta);
    }
    else {
        damage = 0;
        for (double dam : damages) {
            damage += dam;
        }
    }
    if (damage > 1.)
        damage = 1.;
}

} //namespace

