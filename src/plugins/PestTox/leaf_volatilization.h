/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef LEAF_VOLATILIZATION_H
#define LEAF_VOLATILIZATION_H

#include "loss_rate.h"

namespace PestTox {

class LeafVolatilization : public LossRate
{
public:
    LeafVolatilization(QString name, Box *parent);
private:
    // Input
    double VP, Ea, Tref, Tair, load;
    // Output
    double evaporationRate, Tcorrection;
    // Methods
    double computeInstantaneous();
};

} //namespace


#endif
