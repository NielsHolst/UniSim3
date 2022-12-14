/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SOIL_WATER_THRESHOLDS_H
#define SOIL_WATER_THRESHOLDS_H
#include <base/box.h>

namespace coffee {

class SoilWaterThresholds : public base::Box
{
public: 
    SoilWaterThresholds(QString name, QObject *parent);
    void reset();
private:
    // Inputs
    double concWaterMax, propWaterDry, propWaterWilt, propWaterFieldCap, propWaterSaturated;
    // Outputs
    double dry, wilting, fieldCap, saturated;
};

} //namespace
#endif
