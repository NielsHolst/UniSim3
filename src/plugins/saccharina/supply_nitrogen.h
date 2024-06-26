/* Copyright 2019-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Teis Boderskov, Aarhus University [tebo atbios.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SUPPLY_NITROGEN_H
#define SUPPLY_NITROGEN_H
#include "supply_base.h"

namespace saccharina {

class SupplyNitrogen : public SupplyBase
{
public:
    SupplyNitrogen(QString name, Box *parent);
private:
    // Inputs
    double beta, area, waterN;
    // Methods
    void updateValue();
};

}

#endif
