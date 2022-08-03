/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef APHID_ASSIMILATE_H
#define APHID_ASSIMILATE_H

#include "damage.h"

namespace xpest {

class AphidAssimilate : public Damage
{
public:
    AphidAssimilate(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double feedingRate, individualMass, density;
};

} //namespace


#endif
