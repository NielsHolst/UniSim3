/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LEAF_WASH_OFF_H
#define LEAF_WASH_OFF_H

#include "loss_rate.h"

namespace PestTox {

class LeafWashOff : public LossRate
{
public:
    LeafWashOff(QString name, Box *parent);

private:
    // Input
    double rainfall, washOffCoefficient;

    // Methods
    double computeInstantaneous();
};

} //namespace


#endif
