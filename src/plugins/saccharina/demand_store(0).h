/* Copyright 2019-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Teis Boderskov, Aarhus University [tebo atbios.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef DEMAND_STORE_H
#define DEMAND_STORE_H
#include "density.h"

namespace saccharina {

class DemandStore : public Density
{
public:
    DemandStore(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double
        dwStore, dwStructure,
        propStore, propStructure,
        demandStructure,
        concC, concN, concP;
};

}

#endif
