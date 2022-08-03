/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "demand_respiration.h"

using namespace base;

namespace saccharina {

PUBLISH(DemandRespiration)

DemandRespiration::DemandRespiration(QString name, Box *parent)
    : Box(name, parent)
{
    help("calculates temperature-dependent respiration rate");
    Input(resp20).equals(0.01).unit("d-1").help("Respiration rate at 20 oC");
    Input(Q10).equals(1.05).help("For Q10 rule");
    Input(biomassTotal).imports("plant[biomassTotal]");
    Input(T).imports("env[T]");
    Input(timeStep).imports("calendar[timeStepDays]");
    Output(carbon).unit("g C/m").help("Carbon demand for respiration");
}

void DemandRespiration::reset() {
   update();
}

void DemandRespiration::update() {
    double respRate = resp20*pow(Q10, (T-20.)/10.);
    carbon = 12./180.*respRate*biomassTotal*timeStep;
}

}
