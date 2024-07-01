/* Copyright 2019-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Teis Boderskov, Aarhus University [tebo atbios.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SUPPLY_CARBON_H
#define SUPPLY_CARBON_H
#include "supply_base.h"

namespace saccharina {

class SupplyCarbon : public SupplyBase
{
public:
    SupplyCarbon(QString name, Box *parent);
private:
    // Inputs
    double alpha, k,
        lai, czArea, phiN, Id;
    // Outputs
    double
        Iabsorbed, propIabsorbed;
    // Methods
    void updateValue();
};

}

#endif
