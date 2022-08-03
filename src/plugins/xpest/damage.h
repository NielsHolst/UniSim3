/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef DAMAGE_H
#define DAMAGE_H

#include <base/box.h>

namespace xpest {

class Damage : public base::Box
{
public:
    Damage(QString name, QObject *parent);

protected:
    // Outputs
    double damage;
};

} //namespace


#endif
