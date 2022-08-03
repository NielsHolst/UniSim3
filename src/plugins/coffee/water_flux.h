/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef WATER_FLUX_H
#define WATER_FLUX_H
#include <base/box.h>

namespace coffee {

class WaterFlux : public base::Box
{
public: 
    WaterFlux(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double concWaterDry, concWaterWilt, concWaterFieldCap, concWaterSaturated, concWaterMax,
        rootDepth, transpirationCoef, timeStep,
        availableWater, potEvaporation, potTranspiration;
    // Outputs
    double evaporation, transpiration, transpirationRatio;
};

} //namespace
#endif
