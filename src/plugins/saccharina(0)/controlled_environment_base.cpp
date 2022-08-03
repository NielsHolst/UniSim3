/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "controlled_environment_base.h"

using namespace base;

namespace saccharina {

ControlledEnvironmentBase::ControlledEnvironmentBase(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(temperature).unit("oC").help("Water temperature");
    Input(irradiation).unit("micromole PAR/m2/s").help("Irradiation");
    Input(nitrogen).unit("mmol N/L").help("Water nitrogen");
    Input(salinity).unit("PSU").help("Salinity");
    Input(current).unit("m/s").help("Water current");
    Input(lightHours).unit("h").help("Number if lighth hours in a day");
    Output(T).unit("oC").help("Water temperature");
    Output(I).unit("micromole PAR/m2/s").help("Irradiation");
    Output(N).unit("mmol N/L").help("Water nitrogen");
    Output(S).unit("PSU").help("Salinity");
    Output(U).unit("m/s").help("Water current");
}

void ControlledEnvironmentBase::reset() {
    resetNitrogen();
    resetIrradiation();
    T = temperature;
    S = salinity;
    U = current;
    _hour = 0;
}

void ControlledEnvironmentBase::update() {
    ++_hour;
    updateNitrogen();
    updateIrradiation();
}

int ControlledEnvironmentBase::hour() const {
    return _hour;
}

}
