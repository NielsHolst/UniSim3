/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BIOMASS_H
#define BIOMASS_H
#include <base/box.h>

namespace saccharina {

class Biomass : public base::Box
{
public:
    Biomass(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double structuralMass, structuralMassGrowth, Cstruct, Nstruct, kdw, kN, kC, C, N, plantDensity;
    QDate date;
    // Outputs
    double dryWeight, wetWeight, carbonWeight, nitrogenWeight,
        nitrogenPct, carbonPct, dryWeightGrowth, dryWeightYield, wetWeightYield, CNratio,
        growthRatePct, nitrogenUptakeRate;
    // Data
    QDate curDate;
    double previousDryWeight, previousNitrogenWeight;
};

}

#endif
