/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef DEMAND_CARBON_RESPIRATION_H
#define DEMAND_CARBON_RESPIRATION_H
#include <base/box.h>

namespace saccharina {

class DemandCarbonRespiration : public base::Box
{
public:
    DemandCarbonRespiration(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double resp20, Q10, dryWeight, T, timeStep;
    // Outputs
    double value;
};

}

#endif
