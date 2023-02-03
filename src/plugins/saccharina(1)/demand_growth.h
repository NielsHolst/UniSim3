/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef DEMAND_GROWTH_H
#define DEMAND_GROWTH_H
#include <base/box.h>

namespace saccharina {

class DemandGrowth : public base::Box
{
public:
    DemandGrowth(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double maxGrowthRate, massCStruct, propCStruct, propNStruct, fTemp, fSalinity, timeStep;
    // Outputs
    double carbon, nitrogen;
};

}

#endif
