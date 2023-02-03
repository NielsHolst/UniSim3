/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PLANT_RESET_H
#define PLANT_RESET_H
#include <base/box.h>

namespace saccharina {

class PlantReset : public base::Box
{
public:
    PlantReset(QString name, Box *parent);
    void reset();
private:
    // Inputs
    double
        biomassInit, concCInit, concNInit,
        propCStruct, propNStruct, propCRes, propNRes;
    // Outputs
    double biomassCRes, biomassNRes, biomassStruct;
};

}

#endif
