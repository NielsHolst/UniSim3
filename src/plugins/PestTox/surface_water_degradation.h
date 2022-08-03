/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SURFACE_WATER_DEGRADATION_H
#define SURFACE_WATER_DEGRADATION_H

#include <base/box.h>

namespace PestTox {

class SurfaceWaterDegradation  : public base::Box
{
public:
    SurfaceWaterDegradation (QString name, Box *parent);
    void update();

private:
    // Input
    double inflow, DT50, Tw, pondWidth,
    pondHeight, pondLength, inflow2,
    fieldArea, RunoffAmount, fieldWater;

    // Output
    double concentration, outflow,
    kwd, fwdT, pondVolume, runoff, sprayDrift;

};

} //namespace


#endif
