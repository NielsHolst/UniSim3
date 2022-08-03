/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef REDUCTION_H
#define REDUCTION_H

#include "damage.h"

namespace xpest {

class Reduction : public Damage
{
public:
    Reduction(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double pestSeverity, maxDamage, beta;
    QVector<double> damages;
};

} //namespace


#endif
