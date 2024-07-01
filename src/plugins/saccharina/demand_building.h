/* Copyright 2019-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Teis Boderskov, Aarhus University [tebo atbios.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef DEMAND_BUILDING_COST_H
#define DEMAND_BUILDING_COST_H
#include "density.h"

namespace saccharina {

class DemandBuilding : public Density
{
public:
    DemandBuilding(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double demandStructure, demandStoreC, demandStoreN,
        lambdaS, lambdaC, lambdaN,
        concStoreC, constStoreN;
};

}

#endif
