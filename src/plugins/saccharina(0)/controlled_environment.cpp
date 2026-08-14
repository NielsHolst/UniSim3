/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "controlled_environment.h"

using namespace base;

namespace saccharina {

PUBLISH(ControlledEnvironment)

ControlledEnvironment::ControlledEnvironment(QString name, QObject *parent)
    : ControlledEnvironmentBase(name, parent)
{
    help("calculates area loss by erosion");
}

void ControlledEnvironment::resetNitrogen() {
    N = nitrogen;
}

void ControlledEnvironment::resetIrradiation() {
    I = irradiation;
}

void ControlledEnvironment::updateNitrogen() {
    N = nitrogen;
}

void ControlledEnvironment::updateIrradiation(){
    double h = hour()%24;
    I = (h < lightHours) ? irradiation : 0.;
}

}
