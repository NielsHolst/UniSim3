/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef DEMAND_COST_H
#define DEMAND_COST_H
#include <base/box.h>

namespace saccharina {

class DemandCost : public base::Box
{
public:
    DemandCost(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double costStruct, costCRes, costNRes,
        demandGrowthC, demandCRes, demandNRes;
    // Outputs
    double carbon;
};

}

#endif
