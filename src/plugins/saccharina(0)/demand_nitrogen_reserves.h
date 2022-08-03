/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef DEMAND_NITROGEN_RESERVES_H
#define DEMAND_NITROGEN_RESERVES_H
#include <base/box.h>

namespace saccharina {

class DemandNitrogenReserves : public base::Box
{
public:
    DemandNitrogenReserves(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double reservesProportionN, maxReservesProportionN,
        structuralMass, structProportionC, demandStructureC;
    // Outputs
    double value;
};

}

#endif
