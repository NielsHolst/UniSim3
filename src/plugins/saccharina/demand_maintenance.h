/* Copyright 2019-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Teis Boderskov, Aarhus University [tebo atbios.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef DEMAND_MAINTENANCE_H
#define DEMAND_MAINTENANCE_H
#include "density.h"

namespace saccharina {

class DemandMaintenance : public Density
{
public:
    DemandMaintenance(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double biomass, T, r20, Q10;
};

}

#endif
