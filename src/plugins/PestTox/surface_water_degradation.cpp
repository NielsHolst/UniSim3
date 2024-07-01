/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include "surface_water_degradation.h"
#include <cmath>
#include <base/publish.h>
#include "general.h"

using namespace base;
using namespace std;

namespace PestTox {

PUBLISH (SurfaceWaterDegradation )
	
SurfaceWaterDegradation ::SurfaceWaterDegradation (QString name, Box *parent)
	: Box(name, parent)
{
    help("manages fate of pesticide in surface water");
    Input(inflow).unit("g a.i/ha").equals(0.).help("Inflow as runoff from the field");
    Input(inflow2).unit("g a.i/ha").equals(0.).help("Inflow as spray drift off-field edge");
    Input(DT50).unit("day").equals(4.5).help("Biodegradation half-life");
    Input(Tw).unit("Â°C").imports("soilTemperature[value]").help("Water temperature (assumed to be the same as soil temperature)");
    Input(pondWidth).unit("m").equals(1.).help("Width of the pond or channel");
    Input(pondHeight).unit("m").equals(1.).help("Height of the pond or channel");
    Input(pondLength).unit("m").equals(1.).help("Length of the pond or channel");
    Input(fieldArea).unit("m2").imports("field[area]").help("Area of whole field");
    Input(RunoffAmount).unit("mm").equals(0.).help("Runoff amount");

    Output(concentration).unit("mg/L").help("Concetration of the pesticide in the channel or pond");
    Output(outflow).unit("mg/L").help("Pesticide biodegraded");
    Output(fwdT).help("Temperature correction factor");
    Output(kwd).unit("per day").help("Temperature corrected biodegradation rate constant");
    Output(pondVolume).unit("L").help("Volume of the channel or pond");
    Output(fieldWater).unit("L").help("Volume of water runoff from  the field");
    Output(runoff).unit("mg/L").help("Pesticide concentration in the runoff water from the field");
    Output(sprayDrift).unit("mg/L").help("Pesticide concentration in the spray drift off-field edge");
}

void SurfaceWaterDegradation ::update() {
    const double Tref = 25.;
    double k = log(2)/DT50;
    fwdT = (Tw > 0) ? pow(Q10,(Tw - Tref)/10.) : 0;
    kwd = k*fwdT;
    fieldWater = (RunoffAmount > 0) ? fieldArea*(RunoffAmount*1e-3)*1000. : 0;
    runoff = (fieldWater > 0) ? inflow*1000.*fieldArea*1e-4/fieldWater : 0;
    pondVolume = pondWidth*pondHeight*pondLength*1000.;
    sprayDrift = inflow2*1000.*fieldArea*1e-4/pondVolume;
    outflow = concentration*kwd;
    concentration += (fieldWater > 0) ? runoff + sprayDrift - concentration: runoff + sprayDrift - outflow;
}

} //namespace

