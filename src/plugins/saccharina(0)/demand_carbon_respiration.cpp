/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "demand_carbon_respiration.h"

using namespace base;
using phys_math::T0;

namespace saccharina {

PUBLISH(DemandCarbonRespiration)

DemandCarbonRespiration::DemandCarbonRespiration(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates temperature-dependent respiration rate");
    Input(resp20).equals(0.000573).unit("h-1").help("Respiration rate at 20 oC");
    Input(Q10).equals(1.05).help("For Q10 rule");
    Input(dryWeight).imports("biomass[dryWeight]");
    Input(T).imports("env[T]");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Output(value).unit("g C").help("Respiration rate");
}

void DemandCarbonRespiration::reset() {
   // biomass[dryWeight] has not yet been set
   value = 0.;
}

void DemandCarbonRespiration::update() {
    double respRate = resp20*pow(Q10, (T-20.)/10.),
           dt = timeStep/3600.;
    value = 12./180.*respRate*dryWeight*dt;
}

}
