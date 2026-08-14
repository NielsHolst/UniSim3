/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "controlled_environment_chapman.h"

using namespace base;

namespace saccharina {

PUBLISH(ControlledEnvironmentChapman)

ControlledEnvironmentChapman::ControlledEnvironmentChapman(QString name, QObject *parent)
    : ControlledEnvironmentBase(name, parent)
{
    help("calculates area loss by erosion");
    Input(preIrradiation).unit("micromole PAR/m2/s").help("Pre-treatmeant Irradiation");
    Input(preNitrogen).unit("mmol N/L").help("Pre-treatmeant water nitrogen");
    Input(startOfTreatment).unit("h").help("Starting time of treatment");
}

void ControlledEnvironmentChapman::resetNitrogen() {
    N = preNitrogen;
}

void ControlledEnvironmentChapman::resetIrradiation() {
    I = preIrradiation;
}

void ControlledEnvironmentChapman::updateNitrogen() {
    N = (hour() < startOfTreatment) ? preNitrogen : nitrogen;
}

void ControlledEnvironmentChapman::updateIrradiation(){
    double irr = (hour() < startOfTreatment) ? preIrradiation : irradiation;
    double h = hour()%24;
    I = (h < lightHours) ? irr : 0.;
}

}
