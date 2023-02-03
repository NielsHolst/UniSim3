/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PLANT_H
#define PLANT_H
#include <base/box.h>

namespace saccharina {

class Plant : public base::Box
{
public:
    Plant(QString name, Box *parent);
    void reset();
    void update();
private:
    // Primary state variables
    double biomassStruct, biomassCRes, biomassNRes;
    // Inputs
    double
        initBiomassStruct, initBiomassCRes, initBiomassNRes,
        supplyReservesC, supplyReservesN,
        propCStruct, propNStruct, propCRes, propNRes,
        optCConc, optNConc;
    // Outputs
    double biomassTotal, massCStruct, massNStruct, massCRes, massNRes,
        concC, concN;
};

}

#endif
