/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LEAF_UPTAKE_H
#define LEAF_UPTAKE_H

#include "loss_rate.h"

namespace PestTox {

class LeafUptake : public LossRate
{
public:
    LeafUptake(QString name, Box *parent);
private:
    // Input
    QString leafType;
    double Tair, MV;
    // Methods
    double computeInstantaneous();
};

} //namespace


#endif
