/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef DEMAND_CARBON_EXUDATION_H
#define DEMAND_CARBON_EXUDATION_H
#include <base/box.h>

namespace saccharina {

class DemandCarbonExudation : public base::Box
{
public:
    DemandCarbonExudation(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double gamma, proportionC, structuralMass, demandStructure, timeStepSecs, exudation, wetWeight;
    // Outputs
    double value, proportion;
};

}

#endif
