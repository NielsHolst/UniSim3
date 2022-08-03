/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef RUST_DAMAGE_H
#define RUST_DAMAGE_H

#include "damage.h"

namespace xpest {

class RustDamage : public Damage
{
public:
    RustDamage(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double pestSeverity, lai, pustuleArea, pustuleDemand;
};

} //namespace


#endif
