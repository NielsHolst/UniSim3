/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SUPPLY_RESORPTION_H
#define SUPPLY_RESORPTION_H
#include <base/box.h>

namespace saccharina {

class SupplyResorption : public base::Box
{
public:
    SupplyResorption(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double senescentBiomass,
        propCStruct, propNStruct,
        propCResorbed, propNResorbed;
    // Outputs
    double carbon, nitrogen;
};

}

#endif
