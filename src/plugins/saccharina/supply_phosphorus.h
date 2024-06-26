/* Copyright 2019-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Teis Boderskov, Aarhus University [tebo atbios.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SUPPLY_PHOSPHORUS_H
#define SUPPLY_PHOSPHORUS_H
#include "supply_base.h"

namespace saccharina {

class SupplyPhosphorus : public SupplyBase
{
public:
    SupplyPhosphorus(QString name, Box *parent);
private:
    // Inputs
    double gamma, area, waterP;
    // Methods
    void updateValue();
};

}

#endif
