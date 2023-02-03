/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef DEMAND_STRUCTURE_H
#define DEMAND_STRUCTURE_H
#include "density.h"

namespace saccharina {

class DemandStructure : public Density
{
public:
    DemandStructure(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double g, concC, concN, phiT, phiSal, biomass;
};

}

#endif
